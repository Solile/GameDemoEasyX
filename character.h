#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "vector2D.h"
#include "animation.h"
#include "collision_box.h"
#include "state_machine.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

class Character
{
public:
	Character();
	~Character();

	void decrease_hp();

	int get_hp() const
	{
		return hp;
	}
	void set_position(const Vector2D& position)
	{
		this->position = position;
	}
	const Vector2D& get_position() const
	{
		return position;
	}

	void set_velocity(const Vector2D& velocity)
	{
		this->velocity = velocity;
	}

	const Vector2D& get_velocity() const
	{
		return velocity;
	}

	Vector2D get_logic_center() const
	{
		return Vector2D(position.x, position.y - logic_height / 2);
	}

	void set_gravity_enable(bool flag)
	{
		enable_gravity = flag;
	}

	CollisionBox* get_hit_box()
	{
		return hit_box;
	}

	CollisionBox* get_hurt_box()
	{
		return hurt_box;
	}

	bool is_on_floor() const
	{
		return position.y >= FLOOR_Y;
	}

	float get_floor_y() const
	{
		return FLOOR_Y;
	}

	void make_invulnerable()
	{
		is_invulnerable = true;
		timer_invulnerable_status.restart();
	}

	virtual void on_input(const ExMessage& msg);
	virtual void on_update(float delta);
	// 角色渲染
	virtual void on_render();

	virtual void on_hurt();

	void switch_state(const std::string& id);
	void set_animation(const std::string& id);

protected:
	struct AnimationGroup
	{
		Animation left;
		Animation right;
	};

protected:
	// 地板竖直方向的坐标
	const float FLOOR_Y = 620;
	// 重力
	const float GRAVITY = 980 * 2;

protected:
	int hp = 10;
	Vector2D position;
	Vector2D velocity;
	// 角色逻辑高度
	float logic_height = 0;
	bool is_facing_left = true;
	StateMachine state_machine;
	// 启用重力模拟
	bool enable_gravity = true;
	// 当前是否无敌
	bool is_invulnerable = false;
	// 无敌闪烁状态定时器
	Timer timer_invulnerable_blink;
	// 无敌状态定时器
	Timer timer_invulnerable_status;
	// 当前是否处于闪烁的不可见帧
	bool is_blink_invisible = false;
	// 攻击碰撞箱
	CollisionBox* hit_box = nullptr;
	// 受击碰撞箱
	CollisionBox* hurt_box = nullptr;
	// 当前角色动画
	AnimationGroup* current_animation = nullptr;
	// 角色动画池
	std::unordered_map<std::string, AnimationGroup> animation_pool;
};


#endif // !_CHARACTER_H_

