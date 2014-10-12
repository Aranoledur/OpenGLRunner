#pragma once
#include <deque>

namespace EasyGodzilla
{
	class Platform;
	class PlatformManager
	{
	private:
		unsigned int const platformsOnScreen = 5;
		std::deque<Platform*> _platforms;//front is left side. back is right side
		void AddPlatform();

	public:
		PlatformManager();
		~PlatformManager();
		void DrawFrame(float dt);
	};
}