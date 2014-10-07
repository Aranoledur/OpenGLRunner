#pragma once
#include "Helper.h"

namespace EasyGodzilla
{
	class Platform;
	class Game;  // опережающее объ€вление

	class GameDestroyer
	{
	private:
		Game* p_instance;
	public:
		~GameDestroyer();
		void initialize(Game* p);
	};

	class Game
	{
	private:
		GLuint _fragmentShader;
		GLuint _vertexShader;
		GLuint _ebo;
		GLuint _vbo;
		GLint _viewport[4];

		Platform* _platform1;

	public:
		GLuint _globalProgram;
		static const int DELTA_T = 10;
		void Init();
		void DrawFrame();
		int GetWidth();
		int GetHeight();
		void Release();

	private:
		static Game* p_instance;
		static GameDestroyer destroyer;
	protected:
		Game() { }
		Game(const Game&);
		Game& operator=(Game&);
		~Game() { }
		friend class GameDestroyer;
	public:
		static Game& getInstance();
	};
}