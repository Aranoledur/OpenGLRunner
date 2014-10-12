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
		GLint _viewport[4];
		clock_t _previousTime;

		Platform* _platform1;
		
	public:
		GLuint _globalProgram;
		typedef float dtPrecision;
		const clock_t DELTA_T = 0.017 * CLOCKS_PER_SEC;//delta in CLOCKS_PER_SEC
		void Init();
		void DrawFrame();
		int GetWidth();
		int GetHeight();
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