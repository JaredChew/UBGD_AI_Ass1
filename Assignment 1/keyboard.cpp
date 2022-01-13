#include "keyboard.h"

Keyboard::Keyboard(GLFWwindow* const window) : window(window) {

	tick = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::milliseconds(0);
	deltaTime_s = std::chrono::duration<float>(0.0f);

	holdKey = ' ';

	holdDurationUpdate = 500.0f;

}

Keyboard::~Keyboard() { }

bool Keyboard::isPressed(const char& key) {

	return glfwGetKey(window, key);

}

bool Keyboard::isHold(const char& key) {

	if (isPressed(key)) {

		if (deltaTime.count() >= holdDurationUpdate){

			deltaTime = std::chrono::milliseconds(0);

			return true;

		}


		holdKey = key;

	}

	return false;

}

void Keyboard::update() {

	if (isPressed(holdKey) && deltaTime.count() > 0.0f) { deltaTime += std::chrono::duration_cast<std::chrono::milliseconds>(tock - tick); }
	else { deltaTime = std::chrono::milliseconds(0); }

	tock = tick;
	tick = std::chrono::high_resolution_clock::now();

}