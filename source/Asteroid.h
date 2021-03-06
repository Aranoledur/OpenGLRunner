#pragma once
#include "Helper.h"
#include "Vector.hpp"


namespace EasyGodzilla
{
	class Asteroid
	{
		friend class Game;

	private:
		Asteroid(const Vector2d& position);
		GLuint _vbo;

	public:
		float _size;
		float _angle;
		Vector2d _position;
		Vector2d _moveVelocity;

		void tick(Vector2d acel);
		void DrawFrame();
		bool IsSmallest() const;
		void Downgrade();
	};
}

