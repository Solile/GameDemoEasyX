#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "animation.h"

class Player
{
public:
	enum class Facing
	{
		Up,Down,Left,Right
	};

public:
	Player(Atlas* atlas_idle_up, Atlas* atlas_idle_down, Atlas* atlas_idle_left, Atlas* atlas_idle_right,
		Atlas* atlas_run_up, Atlas* atlas_run_down, Atlas* atlas_run_left, Atlas* atlas_run_right)
	{
		animation_idle_up.set_loop(true);
		animation_idle_up.set_interval(0.1f);
		animation_idle_up.add_frame(atlas_idle_up);

		animation_idle_down.set_loop(true);
		animation_idle_down.set_interval(0.1f);
		animation_idle_down.add_frame(atlas_idle_down);

		animation_idle_left.set_loop(true);
		animation_idle_left.set_interval(0.1f);
		animation_idle_left.add_frame(atlas_idle_left);

		animation_idle_right.set_loop(true);
		animation_idle_right.set_interval(0.1f);
		animation_idle_right.add_frame(atlas_idle_right);

		animation_run_up.set_loop(true);
		animation_run_up.set_interval(0.1f);
		animation_run_up.add_frame(atlas_run_up);

		animation_run_down.set_loop(true);
		animation_run_down.set_interval(0.1f);
		animation_run_down.add_frame(atlas_run_down);

		animation_run_left.set_loop(true);
		animation_run_left.set_interval(0.1f);
		animation_run_left.add_frame(atlas_run_left);

		animation_run_right.set_loop(true);
		animation_run_right.set_interval(0.1f);
		animation_run_right.add_frame(atlas_run_right);
	}

	~Player() = default;

	void on_update(float delta)
	{
		if (!position.approx(pos_target))
			velocity = (pos_target - position).normalize() * SPEED_RUN;
		else
			velocity = Vector2D(0, 0);

		if ((pos_target - position).length() <= (velocity * delta).length())
			position = pos_target;
		else
			position += velocity * delta;

		if (velocity.approx(Vector2D(0, 0)))
		{
			switch (facing)
			{
			case Player::Facing::Up:	current_animation = &animation_idle_up;		break;
			case Player::Facing::Down:	current_animation = &animation_idle_down;	break;
			case Player::Facing::Left:	current_animation = &animation_idle_left;	break;
			case Player::Facing::Right: current_animation = &animation_idle_right;	break;
			}
		}
		else
		{
			if (abs(velocity.y) >= 0.0001f)
				facing = (velocity.y > 0) ? Player::Facing::Down : Player::Facing::Up;
			if(abs(velocity.x) >= 0.0001f)
				facing = (velocity.x > 0) ? Player::Facing::Right : Player::Facing::Left;

			switch (facing)
			{
			case Player::Facing::Up:	current_animation = &animation_run_up;		break;
			case Player::Facing::Down:	current_animation = &animation_run_down;	break;
			case Player::Facing::Left:	current_animation = &animation_run_left;	break;
			case Player::Facing::Right: current_animation = &animation_run_right;	break;
			}
		}

		if (!current_animation) return;
		current_animation->set_position(position);
		current_animation->on_update(delta);
	}

	void on_render(const Camera& camera)
	{
		if (!current_animation) return;
		current_animation->on_render(camera);
	}

	void set_position(const Vector2D& position)
	{
		this->position = position;
	}

	const Vector2D& get_position() const
	{
		return position;
	}

	void set_target(const Vector2D& pos_target)
	{
		this->pos_target = pos_target;
	}

protected:
	const float SPEED_RUN = 100.0f;

protected:
	Vector2D pos_target;
	Vector2D position;
	Vector2D velocity;

	Animation animation_idle_up;
	Animation animation_idle_down;
	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_up;
	Animation animation_run_down;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation* current_animation = nullptr;

	Facing facing = Facing::Down;
};


#endif // !_PLAYER_H_
