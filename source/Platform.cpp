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

	Platform::Platform(const Vector2d& position) :
		_position(position),
		_size(biggestSize)
	{
		_angle = (rand() % 31400 / 1000.0f);

		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &eboPoly);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &_ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboPoly);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsPoly), elementsPoly, GL_STATIC_DRAW);

		_posAttribute = glGetAttribLocation(Game::getInstance()._globalProgram, "a_position");
		glEnableVertexAttribArray(_posAttribute);

		_colorAttribute = glGetAttribLocation(Game::getInstance()._globalProgram, "a_color");
		glEnableVertexAttribArray(_colorAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Platform::tick(Vector2d acel)
	{
		Game& gamePrt = EasyGodzilla::Game::getInstance();
		int width = gamePrt.GetWidth();
		int height = gamePrt.GetHeight();

		//acel *= (Game::DELTA_T / 1000.0f);
		_moveVelocity += acel;
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

	glm::mat4 model;

	void Platform::DrawFrame(float dt)
	{
		glUseProgram(Game::getInstance()._globalProgram);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		GLint uniTrans = glGetUniformLocation(Game::getInstance()._globalProgram, "model");

		//draw thin lines
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glVertexAttribPointer(_posAttribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
			glVertexAttribPointer(_colorAttribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

			model = glm::rotate(model, 60 * dt, glm::vec3(0.0f, 0.0f, 1.0f));//60 degree per sec
			glm::mat4 scaleModel;
			scaleModel = glm::scale(scaleModel, glm::vec3(0.5f, 0.5f, 1.0f));

			glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model * scaleModel));
			glDrawElements(GL_LINE_LOOP, sizeof(elements) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//!draw thin lines

		//draw bold lines
		{
			float width = 10;
			glm::mat4 modelBold;
			modelBold = glm::scale(modelBold, glm::vec3(0.5f, width / Game::getInstance().GetHeight(), 1.f));
			glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(modelBold));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboPoly);

			glVertexAttribPointer(_posAttribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
			glVertexAttribPointer(_colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, colorTop);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			modelBold = glm::translate(glm::mat4(), glm::vec3(-0.5f, -0.5f, 1.f));
			modelBold = glm::scale(modelBold, glm::vec3(width / Game::getInstance().GetWidth(), 0.5f, 1.f));
			glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(modelBold));
			glVertexAttribPointer(_colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, colorLeft);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}
		//!draw bold lines
	}

	bool Platform::IsSmallest() const
	{
		return _size <= smallestSize;
	}

	void Platform::Downgrade()
	{
		_size /= downgradeFactor;
	}
}