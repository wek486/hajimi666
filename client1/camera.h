#ifndef CAMERA_H
#define CAMERA_H
#include "vector2.h"
#include "timer.h"
class camera {
public:
	camera() {
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]() {
			is_shaking = false;
			reset();
			});
	};

	~camera() = default;
	const Vector2& get_camera_position()const {
		return camera_position;
	}
	void reset() {
		camera_position.x = 0;
		camera_position.y = 0;
	}
	void on_update(int delta) {
		timer_shake.on_update(delta);
		if (is_shaking) {
			camera_position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			camera_position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}
	void shake(float strength, int duration) {
		is_shaking = true;
		shaking_strength = strength;
		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}
	void look_at(const Vector2& target) {
		position = target - size / 2.0f;
	}
	void set_size(const Vector2& size) {
		this->size = size;
	}
	void set_position(const Vector2& position) {
		this->position = position;
	}
	Vector2 get_position() const {
		return position;
	}
	Vector2 get_size() const {
		return size;
	}

	Vector2 camera_position;
private://如果报错就是权限这里有点问题
	Vector2 size;
	Vector2 position;
	Timer timer_shake;
	bool is_shaking = false;
	float shaking_strength = 0;//抖动幅度


};

#endif
