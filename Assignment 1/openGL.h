#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "bitmap.h"

namespace OpenGL {

	//static void error_callback(int error, const char* description);

	bool initProgramObject_Shader(GLuint &programID, const GLuint& fragmentShader, const GLuint& vertexShade);

	void initTexture(GLuint& textureID, const GLsizei& size, const GLint& resolutionWidth, const GLint& resolutionHeight);

	bool checkIsFramebufferReady();

	GLuint loadShader(GLenum type, const char* shaderSrc);
	GLuint loadShaderFromFile(GLenum shaderType, std::string path);

	void loadTexture(const char* path, GLuint textureID);
	void loadTexture(const char* path, GLuint textureID, const GLint& resolutionWidth, const GLint& resolutionHeight, const GLuint& size);
	void loadCubemapTexture(std::vector<std::string> facesPath, GLuint textureID);

}