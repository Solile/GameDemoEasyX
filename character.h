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
	// ��ɫ��Ⱦ
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
	// �ذ���ֱ���������
	const float FLOOR_Y = 620;
	// ����
	const float GRAVITY = 980 * 2;

protected:
	int hp = 10;
	Vector2D position;
	Vector2D velocity;
	// ��ɫ�߼��߶�
	float logic_height = 0;
	bool is_facing_left = true;
	StateMachine state_machine;
	// ��������ģ��
	bool enable_gravity = true;
	// ��ǰ�Ƿ��޵�
	bool is_invulnerable = false;
	// �޵���˸״̬��ʱ��
	Timer timer_invulnerable_blink;
	// �޵�״̬��ʱ��
	Timer timer_invulnerable_status;
	// ��ǰ�Ƿ�����˸�Ĳ��ɼ�֡
	bool is_blink_invisible = false;
	// ������ײ��
	CollisionBox* hit_box = nullptr;
	// �ܻ���ײ��
	CollisionBox* hurt_box = nullptr;
	// ��ǰ��ɫ����
	AnimationGroup* current_animation = nullptr;
	// ��ɫ������
	std::unordered_map<std::string, AnimationGroup> animation_pool;
};


#endif // !_CHARACTER_H_

