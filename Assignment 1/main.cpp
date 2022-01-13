#define GLFW_INCLUDE_ES2 1
#define GLFW_DLL 1

#define GL_GLEXT_PROTOTYPES

#include <stdlib.h>
#include <stdio.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <GLFW/glfw3.h>

#include "demo.h"
#include "opengl.h"
#include "settings.h"

static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

int main(void) {

	GLFWwindow* window;

	Demo* demo;

	glfwSetErrorCallback(error_callback);

	// Initialize GLFW library
	if (!glfwInit()) { return -1; }

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create and open a window
	window = glfwCreateWindow(

		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		"Assignment 1",
		NULL,
		NULL

	);

	if (!window) {
		glfwTerminate();
		printf("glfwCreateWindow Error\n");
		exit(1);
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	demo = new Demo(window);

	// Repeat
	while (!glfwWindowShouldClose(window)) {

		demo->draw();

		glfwSwapBuffers(window);

		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) { break; }

	}

	delete demo;
	demo = nullptr;

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);

}