#include "Platform.h"
#include "Game.h"
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace EasyGodzilla
{
	static const int biggestSize = 64;
	static const int downgradeFactor = 2;
	static const int downgradeAmount = 2;
	static const int smallestSize = biggestSize / downgradeFactor / downgradeAmount;

	GLfloat vertices[] = {
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
		1.f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-1.f, -1.f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	GLfloat colorTop[] = 
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	GLfloat colorLeft[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLuint elementsPoly[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint eboPoly;

	GLuint elements[] = {
		0, 1, 2, 3
	};

	GLuint Platform::_vbo = 100500;
	GLuint Platform::_ebo = 100500;
	bool Platform::isValid = false;

	void Platform::Init()
	{
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &eboPoly);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &_ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboPoly);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsPoly), elementsPoly, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		isValid = true;
	}

	Platform::Platform(const Vector2d& position, int size) :
		_position(position),
		_size(size)
	{
		_angle = (rand() % 31400 / 1000.0f);
	}

	void Platform::tick(Vector2d acel)
	{
		Game& gamePrt = EasyGodzilla::Game::getInstance();
		int width = gamePrt.GetWidth();
		int height = gamePrt.GetHeight();

		//acel *= (Game::DELTA_T / 1000.0f);
		//_moveVelocity += acel;
		//_position += _moveVelocity * (Game::DELTA_T / 1000.0f);
		float outboundsFactor = 1.2f;
		if (_position._x > width / 2 * outboundsFactor)
			_position._x -= width * outboundsFactor;
		if (_position._y > height / 2 * outboundsFactor)
			_position._y -= height * outboundsFactor;
		if (_position._x < -width / 2 * outboundsFactor)
			_position._x += width * outboundsFactor;
		if (_position._y < -height / 2 * outboundsFactor)
			_position._y += height * outboundsFactor;
		//_angle += 3.0f * Game::DELTA_T / 1000.0f;
	}

	void Platform::DrawFrame(float dt)
	{
		auto viewWidth = Game::getInstance().GetWidth();
		auto viewHeight = Game::getInstance().GetHeight();

		glUseProgram(Game::getInstance()._globalProgram);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		GLint uniTrans = glGetUniformLocation(Game::getInstance()._globalProgram, "model");

		_position._x += _moveDirection._x * Game::gameSpeed * dt;
		_position._y += _moveDirection._y * Game::gameSpeed * dt;
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glVertexAttribPointer(Game::getInstance()._posAttribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
			glVertexAttribPointer(Game::getInstance()._colorAttribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

			float baseXScale = (float)_size / viewWidth;//_size is in our coords. so we convert to view coords
			float baseYScale = 0.15f;//in view coords
			int lineWidth = 4;
			for (int i = 0; i < lineWidth; ++i)
			{
				glm::mat4 localModel;
				float localXScale = baseXScale - (float)i / viewWidth;
				float localYScale = baseYScale - (float)i / viewHeight;
				localModel = glm::scale(glm::mat4(), glm::vec3(localXScale, localYScale, 1.f));
				localModel = glm::translate(localModel, glm::vec3(_position._x / viewWidth / localXScale*2, _position._y / viewHeight / localYScale*2, 0.f));//*2 because 1.f in translate is only viewSide/2, not viewSide
				glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(localModel));
				glDrawElements(GL_LINE_LOOP, sizeof(elements) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	bool Platform::IsSmallest() const
	{
		return _size <= smallestSize;
	}

	void Platform::Downgrade()
	{
		_size /= downgradeFactor;
	}

	void Platform::SetMoveDirection(const Vector2d& direction)
	{
		_moveDirection = direction;
	}
}