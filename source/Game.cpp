#include "Game.h"
#include "Platform.h"
#include "PlatformManager.h"

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

	float Game::xFactor = 1.f;
	float Game::yFactor = 1.f;
	float Game::gameSpeed = 1.f;

	Game::Game()
	{

	}

	void Game::Init()
	{
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

		_posAttribute = glGetAttribLocation(_globalProgram, "a_position");
		glEnableVertexAttribArray(_posAttribute);

		_colorAttribute = glGetAttribLocation(_globalProgram, "a_color");
		glEnableVertexAttribArray(_colorAttribute);

		//our standard resolution.
		_viewport[2] = 960;
		_viewport[3] = 640;

		GLint realResolution [4];
		glGetIntegerv(GL_VIEWPORT, realResolution);
		xFactor = (float)realResolution[2] / _viewport[2];
		yFactor = (float)realResolution[3] / _viewport[3];
		_previousTime = clock();

		Platform::Init();
		_platformMng = new PlatformManager;

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
			_previousTime = currentTime;
			dtPrecision dtInSec = (dtPrecision)dt / (dtPrecision)CLOCKS_PER_SEC;
			_platformMng->DrawFrame(dtInSec);
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
	}
}