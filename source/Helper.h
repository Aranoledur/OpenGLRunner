#pragma once
#include "glwrapper.h"
#include <iostream>

namespace EasyGodzilla
{
	static void checkGlError(const char* op)
	{
		for (GLint error = glGetError(); error; error
			= glGetError()) {
			printf("after %s() glError (0x%x)\n", op, error);
		}
	}

	static void CheckShader(GLuint id, GLuint type, GLint *ret, const char *onfail)
	{
		//Check if something is wrong with the shader
		switch (type) {
		case(GL_COMPILE_STATUS) :
			glGetShaderiv(id, type, ret);
			if (*ret == false){
				int infologLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
				GLchar* buffer = new GLchar[infologLength];
				GLsizei charsWritten = 0;
				std::cout << onfail << std::endl;
				glGetShaderInfoLog(id, infologLength, &charsWritten, buffer);
				std::cout << buffer << std::endl;
				delete buffer;
			}
			break;
		case(GL_LINK_STATUS) :
			glGetProgramiv(id, type, ret);
			if (*ret == false){
				int infologLength = 0;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);
				GLchar* buffer = new GLchar[infologLength];
				GLsizei charsWritten = 0;
				std::cout << onfail << std::endl;
				glGetProgramInfoLog(id, infologLength, &charsWritten, buffer);
				std::cout << buffer << std::endl;
				delete buffer;
			}
			break;
		default:
			break;
		};
	}
}