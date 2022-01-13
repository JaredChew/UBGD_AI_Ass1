#pragma once

#include <GLFW/glfw3.h>

#include <chrono>

#define MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3

class Mouse {

private:

	GLFWwindow* const window;

	std::chrono::steady_clock::time_point tick;
	std::chrono::steady_clock::time_point tock;

	std::chrono::milliseconds deltaTime;
	std::chrono::duration<float> deltaTime_s;

	static double mousePosX;
	static double mousePosY;

	static double posOffsetX;
	static double posOffsetY;

	int holdButton;

	float holdDurationUpdate;

private:

	static void onMouseMove(GLFWwindow* window, double x, double y);

public:
	Mouse(GLFWwindow* window);
	~Mouse();

	void update();

	bool isPressed(const int &button);
	bool isHold(const int &button);

	double getPosX();
	double getPosY();

	double getOffsetPosX();
	double getOffsetPosY();

};