#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <GLFW/glfw3.h>

namespace Objects {

	void drawSkyBox(GLuint texture);
	void drawSquare(GLuint texture);

	/*
	
	#define RECT_VERTICE_W 6
	#define RECT_VERTICE_H 6

	//every quad has 6 vertices
	#define RECT_VERTEX_COUNT RECT_VERTICE_W * RECT_VERTICE_H * 6

	//every vertex has 3 components(x, y z)
	#define RECT_VERTEX_ARRAY_SIZE RECT_VERTEX_COUNT * 3
	#define RECT_UV_ARRAY_SIZE RECT_VERTEX_COUNT * 2

	GLfloat mRectVertices[RECT_VERTEX_ARRAY_SIZE];
	GLfloat mRectUV[RECT_UV_ARRAY_SIZE];
	
	*/

}