#pragma once

struct Vector2d
{
    float _x;
    float _y;
	float getLength() const;
	Vector2d();
	Vector2d(float x, float y);

	static const Vector2d ZERO;

	void normalize();
	float dot(const Vector2d& v) const;
	bool IsZero() const;

	inline const Vector2d operator+(const Vector2d& v) const
	{
		Vector2d result(*this);
		result._x += v._x;
		result._y += v._y;
		return result;
	}
	inline const Vector2d operator-(const Vector2d& v) const
	{
		Vector2d result(*this);
		result._x -= v._x;
		result._y -= v._y;
		return result;
	}

	inline const Vector2d operator-() const
	{
		Vector2d result(*this);
		result._x *= -1;
		result._y *= -1;
		return result;
	}

	inline const Vector2d operator*(float s) const
	{
		Vector2d result(*this);
		result._x *= s;
		result._y *= s;
		return result;
	}

	inline Vector2d& Vector2d::operator*=(float s)
	{
		_x *= s;
		_y *= s;
		return *this;
	}

	inline Vector2d& Vector2d::operator+=(const Vector2d& v)
	{
		_x += v._x;
		_y += v._y;
		return *this;
	}

	inline bool Vector2d::operator==(const Vector2d& v) const
	{
		return _x == v._x && _y == v._y;
	}

};
