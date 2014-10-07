#include "vector.hpp"
#include <math.h>

const Vector2d Vector2d::ZERO = Vector2d(0.0f, 0.0f);

Vector2d::Vector2d():
_x(0.f)
, _y(0.f)
{

}

Vector2d::Vector2d(float x, float y):
_x(x),
_y(y)
{

}

float Vector2d::getLength() const
{
	return sqrt(_x * _x + _y * _y);
}

void Vector2d::normalize()
{
	float n = _x * _x + _y * _y;
	// Already normalized.
	if (n == 1.0f)
		return;

	n = sqrt(n);
	// Too close to zero.
	if (n < 1)
		return;

	n = 1.0f / n;
	_x *= n;
	_y *= n;
}

float Vector2d::dot(const Vector2d& v) const
{
	return (_x * v._x + _y * v._y);
}

bool Vector2d::IsZero() const
{
	return *this == Vector2d::ZERO;
}