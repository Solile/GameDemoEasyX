#ifndef _PATH_H_
#define _PATH_H_

#include "vector2D.h"

#include <vector>

class Path
{
public:
	Path(const std::vector<Vector2D>& point_list)
	{
		this->point_list = point_list;

		for (size_t i = 1; i < point_list.size(); ++i)
		{
			float segment_len = (point_list[i] - point_list[i - 1]).length();
			segment_len_list.push_back(segment_len);
			total_length += segment_len;
		}
	}

	~Path() = default;

	// 获得指定路径坐标
	Vector2D get_postion_at_progress(float progress) const
	{
		if (progress <= 0) return point_list.front();
		if (progress >= 1) return point_list.back();

		float target_distence = total_length * progress;

		float accumulated_len = 0.0f;
		for (size_t i = 1; i < point_list.size(); ++i)
		{
			accumulated_len += segment_len_list[i - 1];
			if (accumulated_len >= target_distence)
			{
				float segment_progress = (target_distence - (accumulated_len - segment_len_list[i - 1])) / segment_len_list[i - 1];
				return point_list[i - 1] + (point_list[i] - point_list[i - 1]) * segment_progress;
			}
		}
		return point_list.back();
	}


private:
	float total_length = 0;
	std::vector<Vector2D> point_list;
	std::vector<float> segment_len_list;
	
};

#endif // !_PATH_H_

