#include "demo.h"

#include <windows.h>
#include <iomanip>
Demo::Demo(GLFWwindow* const window) : window(window) {

	kbd = new Keyboard(window);
	mse = new Mouse(window);

	//Init camera
	camera = new Camera(kbd, 0.0f, 0.0f, 5.5f, 0.0f, 0.0f, 0.0f, 3.0f);

	camera->gPerspectiveMatrix = Matrix4::perspective(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.5f, 400.0f);
	//camera->gOthorMatrix = Matrix4::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 30.0f);

	//Init frame buffer
	glGenFramebuffers(1, &backBuffer);

	//Load shader files
	GLint linked; 

	GLuint vertexShader[SHADER_PROGRAM_COUNT];
	GLuint fragmentShader[SHADER_PROGRAM_COUNT];

	vertexShader[SHADER_PROGRAM_DEFAULT] = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../default.vert");
	fragmentShader[SHADER_PROGRAM_DEFAULT] = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../default.frag");

	vertexShader[SHADER_PROGRAM_GAME_OF_LIFE] = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../default.vert");
	fragmentShader[SHADER_PROGRAM_GAME_OF_LIFE] = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../gameOfLife.frag");

	vertexShader[SHADER_PROGRAM_TEXELMODIFY] = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../default.vert");
	fragmentShader[SHADER_PROGRAM_TEXELMODIFY] = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../texelModify.frag");

	vertexShader[SHADER_PROGRAM_INITTEXTURECOLOUR] = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../default.vert");
	fragmentShader[SHADER_PROGRAM_INITTEXTURECOLOUR] = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../initTextureColour.frag");

	vertexShader[SHADER_PROGRAM_MOUSETOSCREEN] = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../default.vert");
	fragmentShader[SHADER_PROGRAM_MOUSETOSCREEN] = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../mouseToScreen.frag");

	//Init shader programs
	for (int i = 0; i < SHADER_PROGRAM_COUNT; ++i) {

		if (!OpenGL::initProgramObject_Shader(shaderProgramID[i], fragmentShader[i], vertexShader[i])) {
			return;
		}

	}

	//Init textures
	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		OpenGL::initTexture(cellGridTextureID[i], 1, GRID_MAX_WIDTH_DEFAULT, GRID_MAX_HEIGHT_DEFAULT);

	}

	//Init Opengl state
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Init texture base colour
	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		initTextureColour(cellGridTextureID[i], 0.0f, 0.0f, 0.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }

	}

	//init test cell state
	for (int x = 0; x < 10; ++x) {

		editTexel(cellGridTextureID[CELLGRID_TEXTURE_PROCESS], (GRID_MAX_WIDTH_DEFAULT / 2) + x - 4, GRID_MAX_HEIGHT_DEFAULT / 2, 255, 255, 255);
		editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], (GRID_MAX_WIDTH_DEFAULT / 2) + x - 4, GRID_MAX_HEIGHT_DEFAULT / 2, 255, 255, 255);

	}

	tick = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::milliseconds(0);
	deltaTime_s = std::chrono::duration<float>(0.0f);

	cellGridWidth = GRID_MAX_WIDTH_DEFAULT;
	cellGridHeight = GRID_MAX_HEIGHT_DEFAULT;

	resizeWidth = cellGridWidth;
	resizeHeight = cellGridHeight;

	objectSize = (GRID_MAX_WIDTH_DEFAULT * GRID_MAX_HEIGHT_DEFAULT) * 0.000005;

	updateSpeed = 1000.0f;

	cursorPositionX = GRID_MAX_WIDTH_DEFAULT / 2;
	cursorPositionY = GRID_MAX_HEIGHT_DEFAULT / 2;

	cursorPrevPositionX = cursorPositionX;
	cursorPrevPositionY = cursorPositionY;

	editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPositionX, cursorPositionY, 191, 255, 255);

	autoPlay = true;
	inResizeMenu = false;

	std::cout << "\nControls:\nPress ESC to quit.\nAll values changes by 10\nPress R to decrease speed\nPress F to increase speed\nPress Q to zoom out\nPress E to zoom in\nPress P to pause/resume auto simulation\nPress X to reset simulation\nPress Z to enter resize menu\nPress N to enter next state when auto run is off" << "\n";
	std::cout << "Press I, J, K, L to move the cursor\nPress W, A, S, D to move the camera\nPress O to kill/spawn a cell at the cursor" << "\n";
}

Demo::~Demo() {

	delete camera;
	camera = nullptr;

	delete kbd;
	kbd = nullptr;

	delete mse;
	mse = nullptr;

}

void Demo::resizeMenu() {

	if (kbd->isPressed('W')) {

		resizeHeight += DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid height increased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeHeight << "\n";

	}

	if (kbd->isPressed('S')) {

		resizeHeight -= DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid height decreased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeHeight << "\n";

	}

	if (kbd->isPressed('D')) {

		resizeWidth += DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid width increased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeWidth << "\n";

	}

	if (kbd->isPressed('A')) {

		resizeWidth -= DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid width decreased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeWidth << "\n";

	}

	if (kbd->isPressed('E')) {

		resizeCellTexture(resizeWidth, resizeHeight);

		cursorPositionX = resizeWidth / 2;
		cursorPositionY = resizeHeight / 2;

		inResizeMenu = false;

	}

	if (kbd->isPressed('Q')) {

		std::cout << "\nExited resize menu" << "\n";

		inResizeMenu = false;

	}

}

void Demo::cursorControls() {

	if (kbd->isPressed('O')) {
		editTexel(cellGridTextureID[CELLGRID_TEXTURE_PROCESS], cursorPositionX, cursorPositionY, 255, 0, 0);
		editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPositionX, cursorPositionY, 255, 0, 0);
		cursorPrevPositionX = cursorPositionX;
		cursorPrevPositionY = cursorPositionY;
	}

	if (kbd->isPressed('L')) {
		++cursorPositionX;
	}

	if (kbd->isPressed('I')) {
		++cursorPositionY;
	}

	if (kbd->isPressed('J')) {
		--cursorPositionX;
	}

	if (kbd->isPressed('K')) {
		--cursorPositionY;
	}

}

void Demo::controls() {

	if (kbd->isPressed('P')) {

		autoPlay = !autoPlay;

		std::cout << "\nAuto run simulation is " << (autoPlay ? "on" : "off") << "\n";

	}

	if (kbd->isPressed('N')) {

		if (!autoPlay) {

			runSimulation();

			std::cout << "\nNext simulation state " << "\n";

		}

	}

	if (kbd->isPressed('Z') && !autoPlay) {

		inResizeMenu = true;

		std::cout << "\nResize grid menu controls:\nPress Q to quit.\nAll values changes by 10\nPress W to increase height\nPress A to decrease width\nPress S to decrease height\nPress D to increase width\nPress E to apply changes" << "\n";

	}

	if (kbd->isPressed('X') && !autoPlay) {

		glDeleteTextures(TOTAL_CELLGRID_TEXTURE, cellGridTextureID);

		//Init textures
		for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

			OpenGL::initTexture(cellGridTextureID[i], 1, cellGridWidth, cellGridHeight);

		}

		//Init texture base colour
		for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

			initTextureColour(cellGridTextureID[i], 0.0f, 0.0f, 0.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }

		}

		cursorPositionX = cellGridWidth / 2;
		cursorPositionY = cellGridHeight / 2;

		std::cout << "\nSimulation reseted" << "\n";

	}

	if (kbd->isPressed('R')) {

		updateSpeed -= DEFAULT_VALUE_CHANGE;

		std::cout << "\nUpdate speed decreased by " << DEFAULT_VALUE_CHANGE << " and now at " << updateSpeed << "ms" << "\n";

	}

	if (kbd->isPressed('F')) {

		updateSpeed += DEFAULT_VALUE_CHANGE;

		std::cout << "\nUpdate speed increased by " << DEFAULT_VALUE_CHANGE << " and now at " << updateSpeed << "ms" << "\n";

	}

	if (kbd->isPressed('Q')) { objectSize -= DEFAULT_VALUE_CHANGE * (objectSize * 0.01f); }
	if (kbd->isPressed('E')) { objectSize += DEFAULT_VALUE_CHANGE * (objectSize * 0.01f); }

}

void Demo::editTexel(const GLuint& texture, const int& xOffset, const int& yOffset, const uint8_t& r, const uint8_t& g, const uint8_t& b) {

	glBindTexture(GL_TEXTURE_2D, texture);

	uint8_t data[3] = { r, g, b };

	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

}

void Demo::modifyTexelInShader(const GLuint& texture, const float position2D[2], const float colourRGB[3]) {

	glClear(GL_COLOR_BUFFER_BIT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glUseProgram(shaderProgramID[SHADER_PROGRAM_TEXELMODIFY]);

	GLuint position_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_TEXELMODIFY], "position");

	if (position_Loc != -1) { glUniform1fv(position_Loc, 2, position2D); }

	GLuint colour_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_TEXELMODIFY], "colour");

	if (colour_Loc != -1) { glUniform1fv(colour_Loc, 3, colourRGB); }

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_TEXELMODIFY], "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_TEXELMODIFY], "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, Matrix4::identity().data); }

	Objects::drawSquare(texture);

}

void Demo::initTextureColour(const GLuint& texture, const float& r, const float& g, const float& b) {
	
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0, 0, cellGridWidth, cellGridHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, backBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glUseProgram(shaderProgramID[SHADER_PROGRAM_INITTEXTURECOLOUR]);

	GLuint r_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_INITTEXTURECOLOUR], "r");

	if (r_Loc != -1) { glUniform1f(r_Loc, r); }

	GLuint g_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_INITTEXTURECOLOUR], "g");

	if (g_Loc != -1) { glUniform1f(g_Loc, g); }

	GLuint b_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_INITTEXTURECOLOUR], "b");

	if (b_Loc != -1) { glUniform1f(b_Loc, b); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_INITTEXTURECOLOUR], "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, Matrix4::identity().data); }

	Objects::drawSquare(texture);

}

void Demo::manipulateCellState(const GLuint& texture) {

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN]);

	GLuint mousePosXLoc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN], "mousePosX");

	if (mousePosXLoc != -1) { glUniform1f(mousePosXLoc, (float)mse->getPosX()); }

	GLuint mousePosY_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN], "mousePosY");

	if (mousePosY_Loc != -1) { glUniform1f(mousePosY_Loc, (float)mse->getPosY()); }

	GLuint mouseIsPressed_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN], "mouseIsPressed");

	if (mouseIsPressed_Loc != -1) { glUniform1i(mouseIsPressed_Loc, mse->isPressed(MOUSE_BUTTON_LEFT)); }

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN], "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }

	GLuint nonNormalisedMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN], "nonNormalisedMatrix");

	if (nonNormalisedMatrix_Loc != -1) { glUniformMatrix4fv(nonNormalisedMatrix_Loc, 1, GL_FALSE, mvpMatrix.data); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_MOUSETOSCREEN], "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, Matrix4::identity().data); }

	Objects::drawSquare(texture);

}

void Demo::copySquare(const GLuint& texture) {

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID[SHADER_PROGRAM_DEFAULT]);

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_DEFAULT], "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_DEFAULT], "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, Matrix4::identity().data); }

	Objects::drawSquare(texture);

}

void Demo::processSquare(const GLuint& texture) {

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID[SHADER_PROGRAM_GAME_OF_LIFE]);

	GLuint gridSizeX_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_GAME_OF_LIFE], "gridSizeX");

	if (gridSizeX_Loc != -1) { glUniform1f(gridSizeX_Loc, (float)cellGridWidth); }

	GLuint gridSizeY_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_GAME_OF_LIFE], "gridSizeY");

	if (gridSizeY_Loc != -1) { glUniform1f(gridSizeY_Loc, (float)cellGridHeight); }

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_GAME_OF_LIFE], "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_GAME_OF_LIFE], "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, Matrix4::identity().data); }

	Objects::drawSquare(texture);

}

void Demo::renderSquare(const GLuint& texture) {

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID[SHADER_PROGRAM_DEFAULT]);

	mvpMatrix = camera->gPerspectiveMatrix * camera->gViewMatrix * modelMatrix;

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_DEFAULT], "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }

	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID[SHADER_PROGRAM_DEFAULT], "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, mvpMatrix.data); }

	Objects::drawSquare(texture);

}

void Demo::copyToRender() {

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[CELLGRID_TEXTURE_PROCESS], 0);

	copySquare(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY]);

}

void Demo::resizeCellTexture(const float& width, const float& height) {

	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		initTextureColour(cellGridTextureID[i], 0.0f, 0.0f, 0.0f);

	};

	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		//Init textures
		OpenGL::initTexture(cellGridTextureID[i], 1, width, height);

	}

	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		initTextureColour(cellGridTextureID[i], 0.0f, 0.0f, 0.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }

	}

	cellGridWidth = width;
	cellGridHeight = height;

	std::cout << "\nSimulation reseted with new grid size of width " << cellGridWidth << " and height " << cellGridHeight << "\n";

}

void Demo::updateSimulationState() {

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], 0);

	processSquare(cellGridTextureID[CELLGRID_TEXTURE_PROCESS]);

	copyToRender();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], 0);

	manipulateCellState(cellGridTextureID[CELLGRID_TEXTURE_PROCESS]);

	copyToRender();

}

void Demo::runSimulation() {

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, backBuffer);

	if (OpenGL::checkIsFramebufferReady()) {

		glViewport(0, 0, cellGridWidth, cellGridHeight);

		tock = std::chrono::high_resolution_clock::now();

		deltaTime += std::chrono::duration_cast<std::chrono::milliseconds>(tock - tick);
		//deltaTime_s += tock - tick;

		if (deltaTime.count() >= updateSpeed) {

			updateSimulationState();

			deltaTime = std::chrono::milliseconds(0);

		}

		tick = tock;

	}

}

void Demo::draw(void) {

	mse->update();
	kbd->update();

	if (autoPlay) { runSimulation(); }

	if (inResizeMenu) {
		resizeMenu();
	}
	else {
		camera->updateCamera();
		controls();
		cursorControls();
	}

	if (objectSize < 1.0f) { objectSize = 1.0f; }

	if (cursorPositionX != cursorPrevPositionX || cursorPositionY != cursorPrevPositionY) {

		editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPrevPositionX, cursorPrevPositionY, 0, 0, 0);

		cursorPrevPositionX = cursorPositionX;
		cursorPrevPositionY = cursorPositionY;

	}

	editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPositionX, cursorPositionY, 255, 255, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (OpenGL::checkIsFramebufferReady()) {

		glViewport(0, 0, GtexBGWidth, GtexBGHeight);

		modelMatrix = Matrix4::scale(Vector3(objectSize, objectSize, 0.0f));

		renderSquare(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY]);

	}

}