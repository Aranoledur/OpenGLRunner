#include "Game.h"
#include "Platform.h"

// Shader sources
const GLchar* vertexSource =
"attribute vec2 a_position;"
"attribute vec3 a_color;"
"varying vec3 v_color;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 proj;"
"void main() {"
"   v_color = a_color;"
"   gl_Position = model * vec4(a_position, 0.0, 1.0);"
"}";
const GLchar* fragmentSource =
"precision mediump float;\n"
"varying vec3 v_color;"
"uniform mat4 modelColor;"
"void main() {"
"   gl_FragColor = vec4(v_color, 1.0);"
"}";

namespace EasyGodzilla
{
	Game * Game::p_instance = 0;
	GameDestroyer Game::destroyer;

	GameDestroyer::~GameDestroyer() {
		delete p_instance;
	}
	void GameDestroyer::initialize(Game* p) {
		p_instance = p;
	}
	Game& Game::getInstance() {
		if (!p_instance)     {
			p_instance = new Game();
			destroyer.initialize(p_instance);
		}
		return *p_instance;
	}

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
		_globalProgram = glCreateProgram();
		glAttachShader(_globalProgram, _vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(_globalProgram, _fragmentShader);
		checkGlError("glAttachShader");
		glLinkProgram(_globalProgram);
		glUseProgram(_globalProgram);
		checkGlError("glUseProgram");

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(_globalProgram, "a_position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

		GLint colAttrib = glGetAttribLocation(_globalProgram, "a_color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

		glGetIntegerv(GL_VIEWPORT, _viewport);

		_platform1 = new Platform(Vector2d(0, 0));
	}

	void Game::DrawFrame()
	{
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		clock_t currentTime = clock();
		clock_t dt = (currentTime - _previousTime);
		if (dt >= DELTA_T)
		{
			printf("One frame 60 FPS \n");
			_previousTime = currentTime;
			dtPrecision dtInSec = (dtPrecision)dt / (dtPrecision)CLOCKS_PER_SEC;
			_platform1->DrawFrame(dtInSec);
		}
	}

	int Game::GetWidth()
	{
		return _viewport[2];
	}

	int Game::GetHeight()
	{
		return _viewport[3];
	}

	void Game::Release()
	{
		glDeleteProgram(_globalProgram);
		glDeleteShader(_fragmentShader);
		glDeleteShader(_vertexShader);

		glDeleteBuffers(1, &_ebo);
		glDeleteBuffers(1, &_vbo);
	}
}