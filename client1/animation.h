#ifndef ANIMATION_H
#define ANIMATION_H
#include "atlas.h"
#include "util.h"
#include "vector2.h"
#include "functional"
#include <vector>
#include "timer.h"
class Animation {
public:
	enum class AnchorMode {
		Centerd,
		BottomCentered
	};
	//处理对于要裁剪的图片
	struct Frame {
		Rect rect_src;
		IMAGE* image = NULL;
		Frame() = default;
		Frame(IMAGE* image, const Rect& rect_src) :image(image), rect_src(rect_src) {}
		~Frame() = default;
	};
	Animation() {
		timer.set_one_shot(false);
		timer.set_callback([&]() {
			idx_frame++;
			if (idx_frame >= frame_list.size()) {
				idx_frame = is_loop ? 0 : frame_list.size() - 1;
				if (!is_loop && callback) {
					callback();
				}
			}

			});

	};
	~Animation() = default;
	void set_frame_index(size_t idx) {
		// ✨ 如果传入的索引大于或等于当前图集/帧列表的总数，直接拦截，拒绝赋值！
		if (idx >= frame_list.size()) {
			return;
		}
		idx_frame = idx;
	}
	void add_frame(IMAGE* image, int num_h) {

		int width = image->getwidth();
		int height = image->getheight();
		int width_frame = width / num_h;
		for (int i = 0; i < num_h; i++)
		{
			Rect rect_src;
			rect_src.x = i * width_frame, rect_src.y = 0;
			rect_src.w = width_frame, rect_src.h = height;
			frame_list.emplace_back(image, rect_src);//全自动化的添加元素，直接一步到位
		}
	}
	void add_frame(Atlas* atlas) {
		if (!atlas) return;
		for (IMAGE* src_list : atlas->image_list) {

			int width = src_list->getwidth();
			int height = src_list->getheight();
			Rect rect_src;
			rect_src.x = 0, rect_src.y = 0;
			rect_src.w = width, rect_src.h = height;
			frame_list.emplace_back(src_list, rect_src);

		}
	}
	void reset() {
		idx_frame = 0;
		timer.restart();
	}
	void set_anchor_mode(AnchorMode mode) {
		anchor_mode = mode;
	}
	void set_loop(bool flag) {
		is_loop = flag;
	}
	void set_position(const Vector2& position) {
		this->position = position;
	}
	void set_interval(float interval) {
		timer.set_wait_time(interval);
	}
	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}
	void on_update(float delta) {
		timer.on_update(delta);
	}
	void on_render(const camera& camera) {
		// 保命关键！没有帧直接退出，绝不崩溃
		if (frame_list.empty())
			return;
		Frame& frame = frame_list[idx_frame];
		Rect rect_dst;
		rect_dst.x = (int)position.x - frame.rect_src.w / 2;
		rect_dst.y = (int)position.y - frame.rect_src.h / 2;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
		putimage_ex(camera,frame.image, &rect_dst, &frame.rect_src);
	}
private:

	std::vector<Frame> frame_list;//也是为了裁剪图片，然后利用emplace_back函数;
	Timer timer;
	Vector2 position;
	bool is_loop = true;
	size_t idx_frame = 0;
	std::function<void()> callback;//动画结束后的结束逻辑
	AnchorMode anchor_mode = AnchorMode::Centerd;

public:
    int get_frame_count() const { return (int)frame_list.size(); }

};




#endif
