#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

#include <cmath>

class Vector2D
{
public:
	float x = 0;
	float y = 0;
public:
	Vector2D() = default;
	~Vector2D() = default;

	Vector2D(float x, float y)
		:x(x), y(y) { }

	Vector2D operator+(const Vector2D& vec) const
	{
		return Vector2D(x + vec.x, y + vec.y);
	}
	void operator+=(const Vector2D& vec)
	{
		x += vec.x, y += vec.y;
	}
	void operator-=(const Vector2D& vec)
	{
		x -= vec.x, y -= vec.y;
	}
	Vector2D operator-(const Vector2D& vec) const
	{
		return Vector2D(x - vec.x, y - vec.y);
	}
	float operator*(const Vector2D& vec) const
	{
		return x * vec.x + y * vec.y;
	}
	Vector2D operator*(float val) const
	{
		return Vector2D(x * val, y * val);
	}
	float operator*=(float val)
	{
		x *= val, y *= val;
	}
	Vector2D operator/(float val) const
	{
		return Vector2D(x / val, y / val);
	}
	float operator/=(float val)
	{
		x /= val, y /= val;
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2D normalize()
	{
		float len = length();
		if (len == 0)
			return Vector2D(0, 0);

		return Vector2D(x / len, y / len);
	}

	bool approx(const Vector2D& target)
	{
		return (*this - target).length() <= 0.0001f;
	}
};

#endif // !_VECTOR2D_H_

