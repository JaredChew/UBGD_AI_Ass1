#include "mouse.h"

 double Mouse::mousePosX;
 double Mouse::mousePosY;

 double Mouse::posOffsetX;
 double Mouse::posOffsetY;

Mouse::Mouse(GLFWwindow* window) : window(window) {

	tick = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::milliseconds(0);
	deltaTime_s = std::chrono::duration<float>(0.0f);

	mousePosX = -1.0;
	mousePosY = -1.0;

	posOffsetX = 0.0;
	posOffsetY = 0.0;

	holdDurationUpdate = 500.0f;

	holdButton = NULL;

	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

Mouse::~Mouse() { }

void Mouse::onMouseMove(GLFWwindow* window, double x, double y) {

	if (mousePosX == -1 && mousePosY == -1)  {

		mousePosX = x;
		mousePosY = y;

		return;

	}

	posOffsetX = x - mousePosX;
	posOffsetY = y - mousePosY;

	mousePosX = x;
	mousePosY = y;

}

bool Mouse::isPressed(const int &button) {

	return glfwGetMouseButton(window, button);

}

bool Mouse::isHold(const int& button) {

	if (isPressed(button)) {

		if (deltaTime.count() >= holdDurationUpdate) {

			deltaTime = std::chrono::milliseconds(0);

			return true;

		}


		holdButton = button;

	}

	return false;

}

void Mouse::update() {

	if (isPressed(holdButton) && deltaTime.count() > 0.0f) { deltaTime += std::chrono::duration_cast<std::chrono::milliseconds>(tock - tick); }
	else { deltaTime = std::chrono::milliseconds(0); }

	tock = tick;
	tick = std::chrono::high_resolution_clock::now();

}

double Mouse::getPosX() { return mousePosX; }

double Mouse::getPosY() { return mousePosY; }

double Mouse::getOffsetPosX() { return posOffsetX; }

double Mouse::getOffsetPosY() { return posOffsetY; }