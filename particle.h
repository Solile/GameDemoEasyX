#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "atlas.h"
#include "vector2D.h"
#include "camera.h"
#include "util.h"

class Particle
{
public:
	Particle() = default;

	Particle(const Vector2D& position, Atlas* atlas, int lifespan):
		position(position),atlas(atlas),lifespan(lifespan){ }

	void set_atlas(Atlas* new_atlas) { atlas = new_atlas; }

	void set_position(const Vector2D& new_position) { position = new_position; }

	void set_lifespan(int ms) { lifespan = ms; }

	bool check_valid() const
	{
		return valid;
	}

	void on_update(int delta)
	{
		timer += delta;
		if (timer >= lifespan)
		{
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size())
			{
				idx_frame = atlas->get_size() - 1;
				valid = false;
			}
		}
	}

	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
	}

	~Particle() = default;

private:
	int timer = 0;		// 粒子动画播放定时器
	int lifespan = 0;	// 单帧例子动画持续时长
	int idx_frame = 0;	// 当前正在播放的动画帧
	Vector2D position;
	bool valid = true;
	Atlas* atlas = nullptr;
};



#endif // !_PARTICLE_H_

