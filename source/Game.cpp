#include "Game.h"

// Shader sources
const GLchar* vertexSource =
"attribute vec2 position;"
"attribute vec3 color;"
"varying vec3 Color;"
"void main() {"
"   Color = color;"
"   gl_Position = vec4(position, 0.0, 1.0);"
"}";
const GLchar* fragmentSource =
"precision mediump float;\n"
"varying vec3 Color;"
"void main() {"
"   gl_FragColor = vec4(Color, 1.0);"
"}";

namespace EasyGodzilla
{
	void Game::Init()
	{
		// Create a Vertex Buffer Object and copy the vertex data to it
		glGenBuffers(1, &_vbo);

		GLfloat vertices[] = {
			-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
			-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
		};

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Create an element array
		glGenBuffers(1, &_ebo);

		GLuint elements[] = {
			0, 1, 2,
			2, 3, 0
		};

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		// Create and compile the vertex shader
		_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(_vertexShader, 1, &vertexSource, NULL);
		glCompileShader(_vertexShader);

		// Create and compile the fragment shader
		_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(_fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(_fragmentShader);

		GLint ret;
		CheckShader(_vertexShader, GL_COMPILE_STATUS, &ret, "unable to compile the vertex shader!");
		CheckShader(_fragmentShader, GL_COMPILE_STATUS, &ret, "unable to compile the fragment shader!");

		// Link the vertex and fragment shader into a shader program
		_shaderProgram = glCreateProgram();
		glAttachShader(_shaderProgram, _vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(_shaderProgram, _fragmentShader);
		checkGlError("glAttachShader");
		glLinkProgram(_shaderProgram);
		glUseProgram(_shaderProgram);
		checkGlError("glUseProgram");

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(_shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

		GLint colAttrib = glGetAttribLocation(_shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	}

	void Game::DrawFrame()
	{
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a rectangle from the 2 triangles using 6 indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Game::Release()
	{
		glDeleteProgram(_shaderProgram);
		glDeleteShader(_fragmentShader);
		glDeleteShader(_vertexShader);

		glDeleteBuffers(1, &_ebo);
		glDeleteBuffers(1, &_vbo);
	}
}