#pragma once

#include <chrono>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <GLFW/glfw3.h>

#include "angle_util/Matrix.h"
#include "angle_util/geometry_utils.h"

#include "camera.h"
#include "opengl.h"
#include "objects.h"
#include "settings.h"
#include "keyboard.h"
#include "mouse.h"
#include "demo_specification.h"

class Demo {

private:

	GLuint backBuffer;

	Matrix4 mvpMatrix;
	Matrix4 modelMatrix;

	GLuint shaderProgramID[SHADER_PROGRAM_COUNT] = { (GLuint)-1 };

	GLint GtexBGWidth = WINDOW_WIDTH / 1;
	GLint GtexBGHeight = WINDOW_HEIGHT / 1;

	Camera* camera;

	Keyboard* kbd;
	Mouse* mse;

	GLFWwindow* const window;

	std::chrono::steady_clock::time_point tick;
	std::chrono::steady_clock::time_point tock;

	std::chrono::milliseconds deltaTime;
	std::chrono::duration<float> deltaTime_s;

	GLuint cellGridTextureID[TOTAL_CELLGRID_TEXTURE];

	int cellGridWidth;
	int cellGridHeight;

	float objectSize;

	float updateSpeed;

	float resizeWidth;
	float resizeHeight;

	int cursorPositionX;
	int cursorPositionY;

	int cursorPrevPositionX;
	int cursorPrevPositionY;

	bool autoPlay;
	bool inResizeMenu;

private:

	void editTexel(const GLuint& texture, const int& xOffset, const int& yOffset, const uint8_t& r, const uint8_t& g, const uint8_t& b);
	void modifyTexelInShader(const GLuint& texture, const float position2D[2], const float colourRGB[3]);

	void initTextureColour(const GLuint& texture, const float& r, const float& g, const float& b);

	void copySquare(const GLuint& texture);
	void renderSquare(const GLuint& texture);
	void processSquare(const GLuint& texture);

	void copyToRender();
	void manipulateCellState(const GLuint& texture);
	void updateSimulationState();
	void runSimulation();

	void resizeCellTexture(const float& width, const float& height);
	void resizeMenu();

	void cursorControls();
	void controls();

public:

	Demo(GLFWwindow* const window);
	~Demo();

	void draw();

};