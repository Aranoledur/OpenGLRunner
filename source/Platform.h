#pragma once
#include "Helper.h"
#include "Vector.hpp"


namespace EasyGodzilla
{
	class Platform
	{
		friend class Game;
	private:
		static GLuint _vbo;
		static GLuint _ebo;
		Vector2d _moveDirection;
		static void Init();

	public:
		int _size;
		float _angle;
		Vector2d _position;

		static bool isValid;
		Platform(const Vector2d& position, int size);
		void tick(Vector2d acel);
		void DrawFrame(float dt);
		bool IsSmallest() const;
		void Downgrade();
		void SetMoveDirection(const Vector2d& direction);
	};
}

