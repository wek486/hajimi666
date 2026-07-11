#ifndef TIMER_H
#define TIMER_H
#include <functional>

class Timer {
public:
	Timer(float time=0.0f) :wait_time(time) {
		pass_time = 0.0f;
		is_one_shooted = false;
		is_shooted = false;
		is_paused = false;
	};
	~Timer() = default;
	void setWaitTime(float time) {
		wait_time = time;

	}
	void update(float deltaTime) {
		if (is_paused) return;
		pass_time += deltaTime;
		if (pass_time >= wait_time ) {
			bool can_shooted = !is_one_shooted || (!is_shooted && is_one_shooted);
			if (can_shooted) {
				is_shooted = true;
				if (can_shooted&&callback) callback();
			}
		}
	}
	void set_pause(bool pause) {
		is_paused = pause;
	}
	void set_one_shooted(bool is_one_shooted) {
		this->is_one_shooted = is_one_shooted;

	}
	void set_resum() {
		is_paused = false;
	}
	void reset() {
		pass_time = 0;
		is_shooted = false;
	}
	void set_callback(std::function<void()>callback) {
		this->callback = callback;
	}
private:
	float wait_time;
	float pass_time;
	bool is_one_shooted;
	bool is_shooted;
	bool is_paused;
	std::function<void()>callback;
};



#endif