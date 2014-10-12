#include "PlatformManager.h"
#include "Platform.h"
#include "Game.h"

namespace EasyGodzilla
{
	PlatformManager::PlatformManager()
	{
		assert(Platform::isValid);
		for (int i = 0; i < platformsOnScreen; ++i)
		{
			AddPlatform();
		}
	}

	PlatformManager::~PlatformManager()
	{
		while (!_platforms.empty())
		{
			delete _platforms.back();
			_platforms.pop_back();
		}
	}

	void PlatformManager::AddPlatform()
	{
		auto viewWidth = Game::getInstance().GetWidth();
		auto viewHeight = Game::getInstance().GetHeight();
		int size = viewWidth*0.3f;
		int xPos = -250;
		if (!_platforms.empty())
		{
			xPos = _platforms.back()->_position._x + size*1.2f;
		}
		Platform* platform = new Platform(Vector2d(xPos, -viewHeight*0.4f), size);
		platform->SetMoveDirection(Vector2d(-50.f, 0.f));
		_platforms.push_back(platform);
	}

	void PlatformManager::DrawFrame(float dt)
	{
		auto viewWidth = Game::getInstance().GetWidth();
		auto viewHeight = Game::getInstance().GetHeight();

		assert(!_platforms.empty());
		auto backPlatform = _platforms.front();
		if (backPlatform->_position._x + backPlatform->_size < -viewWidth / 2)
		{
			delete backPlatform;
			_platforms.pop_front();
			AddPlatform();
		}

		for (auto platform : _platforms)
		{
			platform->DrawFrame(dt);
		}

	}
}