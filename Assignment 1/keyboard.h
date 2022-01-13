#pragma once

#include <GLFW/glfw3.h>

#include <chrono>

class Keyboard {

private:

	GLFWwindow* const window;

	std::chrono::steady_clock::time_point tick;
	std::chrono::steady_clock::time_point tock;

	std::chrono::milliseconds deltaTime;
	std::chrono::duration<float> deltaTime_s;

	char holdKey;

	float holdDurationUpdate;

public:
	Keyboard(GLFWwindow* const window);
	~Keyboard();

	bool isPressed(const char &key);
	bool isHold(const char &key);

	void update();

};