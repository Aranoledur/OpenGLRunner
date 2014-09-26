#pragma once
#include "Helper.h"

namespace EasyGodzilla
{
	class Game
	{
	private:
		GLuint _shaderProgram;
		GLuint _fragmentShader;
		GLuint _vertexShader;

		GLuint _ebo;
		GLuint _vbo;

	public:
		void Init();
		void DrawFrame();
		void Release();
	};
}