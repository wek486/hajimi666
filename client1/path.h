#ifndef PATH_H
#define PATH_H
#include "vector2.h"
#include <vector>
class Path {
public:
	Path(const std::vector<Vector2>& point_list) {
		this->point_list = point_list;
		total_len = 0.0f; // ? 确保清零
		segment_len_list.clear();
		for (size_t i = 1; i < point_list.size(); i++) {
			float segment_len = (point_list[i] - point_list[i - 1]).length();
			segment_len_list.push_back(segment_len); // ? 将每一段长度存入 vector
			total_len += segment_len;                // ? 累加总长度
		}
	}
	~Path() = default;
	Vector2 get_position_at_progress(float progress)const  {
		if (progress <= 0)return point_list.front();
		if (progress >= 1)return point_list.back();
		float target_len = total_len * progress;
		float accumulated_len = 0.0f;
		for (size_t i = 1; i<point_list.size();i++ ) {
			accumulated_len += segment_len_list[i - 1];
			if (accumulated_len>=target_len) {
				float segment_len = accumulated_len - segment_len_list[i - 1];//超过的这一段的理论实际上的超过距离，即距离上一个点还需出发的距离
				float segment_progress = (target_len - segment_len) / segment_len_list[i - 1];
				return point_list[i - 1] + (point_list[i] - point_list[i - 1]) * segment_progress;

			}
		}
		return point_list.back();
	}


private: 
	std::vector<Vector2> point_list;
	std::vector<float> segment_len_list;//存储每一段的长度
	float total_len = 0.0f;




};




#endif