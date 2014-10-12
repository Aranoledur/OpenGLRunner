#pragma once
#include "Helper.h"
#include <time.h>

namespace EasyGodzilla
{
	class PlatformManager;
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
		GLint _viewport[4];
		clock_t _previousTime;

		PlatformManager* _platformMng;
		
	public:
		GLuint _globalProgram;
		GLuint _posAttribute;
		GLuint _colorAttribute;
		typedef float dtPrecision;
		const clock_t DELTA_T = 0.017 * CLOCKS_PER_SEC;//delta in CLOCKS_PER_SEC
		void Init();
		void DrawFrame();
		int GetWidth();
		int GetHeight();
		static float xFactor;
		static float yFactor;
		static float gameSpeed;
		void Release();

	private:
		static Game* p_instance;
		static GameDestroyer destroyer;
	protected:
		Game();
		friend class GameDestroyer;
	public:
		static Game& getInstance();
	};
}