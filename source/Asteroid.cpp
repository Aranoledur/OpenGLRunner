#include "Asteroid.h"
#include "Game.h"
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>
//#include "glm/vec2.hpp"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"

namespace EasyGodzilla
{
	static const int biggestSize = 64;
	static const int downgradeFactor = 2;
	static const int downgradeAmount = 2;
	static const int smallestSize = biggestSize / downgradeFactor / downgradeAmount;

	static const GLfloat vertices[] = 
	{
		sin(2 * M_PI / 5 * 0), cos(2 * M_PI / 5 * 0),
		sin(2 * M_PI / 5 * 1), cos(2 * M_PI / 5 * 1),
		sin(2 * M_PI / 5 * 2), cos(2 * M_PI / 5 * 2),
		sin(2 * M_PI / 5 * 3), cos(2 * M_PI / 5 * 3),
		sin(2 * M_PI / 5 * 4), cos(2 * M_PI / 5 * 4),
		sin(2 * M_PI / 5 * 5), cos(2 * M_PI / 5 * 5),
	};

	static const float color[] =
	{
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f
	};

	Asteroid::Asteroid(const Vector2d& position) :
		_position(position),
		_size(biggestSize)
	{
		_angle = (rand() % 31400 / 1000.0f);

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	}

	void Asteroid::tick(Vector2d acel)
	{
		Game& gamePrt = EasyGodzilla::Game::getInstance();
		int width = gamePrt.GetWidth();
		int height = gamePrt.GetHeight();

		acel *= (Game::DELTA_T / 1000.0f);
		_moveVelocity += acel;
		_position += _moveVelocity * (Game::DELTA_T / 1000.0f);
		float outboundsFactor = 1.2f;
		if (_position._x > width / 2 * outboundsFactor)
			_position._x -= width * outboundsFactor;
		if (_position._y > height / 2 * outboundsFactor)
			_position._y -= height * outboundsFactor;
		if (_position._x < -width / 2 * outboundsFactor)
			_position._x += width * outboundsFactor;
		if (_position._y < -height / 2 * outboundsFactor)
			_position._y += height * outboundsFactor;
		_angle += 3.0f * Game::DELTA_T / 1000.0f;
	}

	void Asteroid::DrawFrame()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		tick(Vector2d(10.f, 10.f));

		//glm::mat4 model;
		//model = glm::rotate(model, _angle, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(_position.x, _position.y, 1.0f));
		//model = glm::scale(glm::mat4(1.0f), glm::vec3(_size, _size, 1.0f));

		GLint a_position = glGetAttribLocation(EasyGodzilla::Game::getInstance()._globalProgram, "a_position");
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);

		GLint a_color = glGetAttribLocation(EasyGodzilla::Game::getInstance()._globalProgram, "a_color");
		glEnableVertexAttribArray(a_color);
		glVertexAttribPointer(a_color, 3, GL_FLOAT, GL_FALSE, 0, color);

		//GLint modelHandle = glGetUniformLocation(EasyGodzilla::Game::getInstance()._globalProgram, "model");
		//glUniformMatrix4fv(modelHandle, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 3);

	}

	bool Asteroid::IsSmallest() const
	{
		return _size <= smallestSize;
	}

	void Asteroid::Downgrade()
	{
		_size /= downgradeFactor;
	}
}