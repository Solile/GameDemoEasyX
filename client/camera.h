#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector2D.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void set_size(const Vector2D size)
	{
		this->size = size;
	}

	const Vector2D& get_size() const
	{
		return size;
	}

	void set_position(const Vector2D position)
	{
		this->position = position;
	}

	const Vector2D& get_position() const
	{
		return position;
	}

	void look_at(const Vector2D& target)
	{
		position = target - size / 2.0f;
	}

private:
	Vector2D size;
	Vector2D position;
};

#endif // !_CAMERA_H_

