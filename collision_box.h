#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "vector2D.h"
#include "collision_layer.h"

#include <functional>

class CollisionBox
{
	friend class CollisionManager;

public:
	void set_enable(bool flag)
	{
		enabled = flag;
	}
	void set_layer_src(CollisionLayer layer)
	{
		layer_src = layer;
	}
	void set_layer_dst(CollisionLayer layer)
	{
		layer_dst = layer;
	}
	void set_on_collide(std::function<void()> on_collide)
	{
		this->on_collide = on_collide;
	}
	void set_size(const Vector2D& size)
	{
		this->size = size;
	}
	const Vector2D get_size() const
	{
		return size;
	}
	void set_position(const Vector2D& position)
	{
		this->position = position;
	}

private:
	Vector2D size;
	Vector2D position;
	bool enabled = true;
	std::function<void()> on_collide;
	// 当前目标所处的目标层
	CollisionLayer layer_src = CollisionLayer::None;
	// 与之碰撞的目标层
	CollisionLayer layer_dst = CollisionLayer::None;

private:
	CollisionBox() = default;
	~CollisionBox() = default;

};

#endif // !_COLLISION_BOX_H_

