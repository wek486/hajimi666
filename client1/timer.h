#ifndef TIMER_H
#define TIMER_H
#include<functional>
class Timer {
public:
	void restart() {
		pass_time = 0;
		shotted = false;
		paused = false;
	}
    void set_wait_time(float vealta) {
        wait_time = vealta;
	}
	void set_one_shot(bool flag) {
		one_shot = flag;
	}
	void pause() {
		paused = true;
	}
	void resume() {
		paused = false;
	}
    void on_update(float delta) {
        if (paused) {
            return;
        }
        pass_time += delta;
        if (pass_time >= wait_time) {
            if ((!one_shot || (one_shot && !shotted)) && callback) {
                callback();
            }
            shotted = true;
            pass_time = 0.0f;
            if (one_shot) paused = true;
        }
    }
	Timer() = default;
	~Timer() = default;
	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	};

private:
    float pass_time = 0.0f;
    float wait_time = 0.0f;
    bool paused = false;
    bool shotted = false;//是否触发
    bool one_shot = false;//是否单次触发
    std::function<void()> callback;
};
#endif 
