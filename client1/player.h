#ifndef PLAYER_H
#define PLAYER_H
#include "state_machine.h"
#include   "Vector2.h"
#include "animation.h"
#include "player_state_node.h"
#include <graphics.h>
#include "resources_manager.h"
#define speed_run  120.0f // pixels per second; increased from 5 to make movement faster
extern int id_player;
struct AnimationGroup {
	Animation left;
	Animation right;
	Animation up;
	Animation down;
};
enum class facing {
	left, right, up, down
};
class Player {
public:
	Player() {
		AnimationGroup& hajimi_animation_idle = animation_pool["hajimi_idle"];

		{
			hajimi_animation_idle.down.set_interval(0.1f);
			hajimi_animation_idle.down.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_idle_down");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_idle_down" << std::endl;
			hajimi_animation_idle.down.add_frame(a);
		}
		{
			hajimi_animation_idle.up.set_interval(0.1f);
			hajimi_animation_idle.up.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_idle_up");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_idle_up" << std::endl;
			hajimi_animation_idle.up.add_frame(a);
		}
		{
			hajimi_animation_idle.left.set_interval(0.1f);
			hajimi_animation_idle.left.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_idle_left");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_idle_left" << std::endl;
			hajimi_animation_idle.left.add_frame(a);
		}
		{
			hajimi_animation_idle.right.set_interval(0.1f);
			hajimi_animation_idle.right.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_idle_right");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_idle_right" << std::endl;
			hajimi_animation_idle.right.add_frame(a);
		}
		AnimationGroup & hajimi_animation_run = animation_pool["hajimi_run"];
		{
			hajimi_animation_run.down.set_interval(0.1f);
			hajimi_animation_run.down.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_run_down");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_run_down" << std::endl;
			hajimi_animation_run.down.add_frame(a);
		}
		{
			hajimi_animation_run.up.set_interval(0.1f);
			hajimi_animation_run.up.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_run_up");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_run_up" << std::endl;
			hajimi_animation_run.up.add_frame(a);
		}
		{
			hajimi_animation_run.left.set_interval(0.1f);
			hajimi_animation_run.left.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_run_left");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_run_left" << std::endl;
			hajimi_animation_run.left.add_frame(a);
		}
		{
			hajimi_animation_run.right.set_interval(0.1f);
			hajimi_animation_run.right.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_run_right");
			if (!a) std::cerr << "[Player] missing atlas: hajimi_run_right" << std::endl;
			hajimi_animation_run.right.add_frame(a);
		}
		AnimationGroup& manbo_animation_idle = animation_pool["manbo_idle"];
		{
			manbo_animation_idle.down.set_interval(0.1f);
			manbo_animation_idle.down.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_idle_down");
			if (!a) std::cerr << "[Player] missing atlas: manbo_idle_down" << std::endl;
			manbo_animation_idle.down.add_frame(a);
		}
		{
			manbo_animation_idle.up.set_interval(0.1f);
			manbo_animation_idle.up.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_idle_up");
			if (!a) std::cerr << "[Player] missing atlas: manbo_idle_up" << std::endl;
			manbo_animation_idle.up.add_frame(a);
		}
		{
			manbo_animation_idle.left.set_interval(0.1f);
			manbo_animation_idle.left.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_idle_left");
			if (!a) std::cerr << "[Player] missing atlas: manbo_idle_left" << std::endl;
			manbo_animation_idle.left.add_frame(a);
		}
		{
			manbo_animation_idle.right.set_interval(0.1f);
			manbo_animation_idle.right.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_idle_right");
			if (!a) std::cerr << "[Player] missing atlas: manbo_idle_right" << std::endl;
			manbo_animation_idle.right.add_frame(a);
		}
		AnimationGroup& manbo_animation_run = animation_pool["manbo_run"];
		{
			manbo_animation_run.down.set_interval(0.1f);
			manbo_animation_run.down.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_run_down");
			if (!a) std::cerr << "[Player] missing atlas: manbo_run_down" << std::endl;
			manbo_animation_run.down.add_frame(a);
		}
		{
			manbo_animation_run.up.set_interval(0.1f);
			manbo_animation_run.up.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_run_up");
			if (!a) std::cerr << "[Player] missing atlas: manbo_run_up" << std::endl;
			manbo_animation_run.up.add_frame(a);
		}
		{
			manbo_animation_run.left.set_interval(0.1f);
			manbo_animation_run.left.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_run_left");
			if (!a) std::cerr << "[Player] missing atlas: manbo_run_left" << std::endl;
			manbo_animation_run.left.add_frame(a);
		}
		{
			manbo_animation_run.right.set_interval(0.1f);
			manbo_animation_run.right.set_loop(true);
			Atlas* a = ResourcesManager::instance()->find_atlas("manbo_run_right");
			if (!a) std::cerr << "[Player] missing atlas: manbo_run_right" << std::endl;
			manbo_animation_run.right.add_frame(a);
		}
		{
			state_machine.register_state("idle", new PlayerIdleState());
			state_machine.register_state("run", new PlayerRunState());
			state_machine.set_entry("idle");
		}
		//可以删了
	    //
		//
		set_animation(
			"hajimi_idle"
		);
	};
	~Player() = default;
	void on_update(float delta) {
		// improved movement: move by speed * delta, avoid normalizing zero vector
		Vector2 to_target = target_position - position;
		float dist = to_target.length();
		if (dist <= 0.001f) {
			// already at target
			position = target_position;
			velocity = Vector2(0, 0);
		} else {
			Vector2 dir = to_target / dist; // safe since dist > 0
			float move = (float)speed_run * delta;
			if (move >= dist) {
				position = target_position;
				velocity = Vector2(0, 0);
			} else {
				position += dir * move;
				velocity = dir * (float)speed_run;
			}
		}
		state_machine.on_update(delta);
		if (current_animation_group) {
			if (velocity.approx(Vector2(0, 0))) 
			{
				switch (facing_direction) {
				case facing::up:
					current_animation =& current_animation_group->up;
					break;
				case facing::down:
					current_animation = &current_animation_group->down;
					break;
				case facing::left:
					current_animation = &current_animation_group->left;
					break;
				case facing::right:
					current_animation = &current_animation_group->right;
					break;
				}
			}
			else {
			// choose dominant movement axis to determine facing
			float ax = fabs(velocity.x);
			float ay = fabs(velocity.y);
			const float thresh = 0.001f;
			if (ax > ay && ax >= thresh) {
				facing_direction = (velocity.x > 0) ? facing::right : facing::left;
			}
			else if (ay >= thresh) {
				// screen Y increases downward: positive y -> down
				facing_direction = (velocity.y > 0) ? facing::up : facing::down;//这里是反的，EasyX里面纵坐标，反的
			}
				switch (facing_direction) {
				case facing::up:
					current_animation = &current_animation_group->up;
					break;
				case facing::down:
					current_animation = &current_animation_group->down;
					break;
				case facing::left:
					current_animation = &current_animation_group->left;
					break;
				case facing::right:
					current_animation = &current_animation_group->right;
					break;
				}

				// if chosen direction has no frames, try fallbacks to avoid missing animation
				if (current_animation && current_animation->get_frame_count() == 0) {
					// preferred fallback order: horizontal mirror, vertical, any
					Animation* fallback = nullptr;
					if (facing_direction == facing::right || facing_direction == facing::left) {
						// try opposite horizontal
						fallback = (facing_direction == facing::right) ? &current_animation_group->left : &current_animation_group->right;
						if (fallback->get_frame_count() == 0) fallback = &current_animation_group->down;
						if (fallback->get_frame_count() == 0) fallback = &current_animation_group->up;
					} else {
						// vertical preferred
						fallback = (facing_direction == facing::down) ? &current_animation_group->up : &current_animation_group->down;
						if (fallback->get_frame_count() == 0) fallback = &current_animation_group->left;
						if (fallback->get_frame_count() == 0) fallback = &current_animation_group->right;
					}
					if (fallback && fallback->get_frame_count() > 0) {
						current_animation = fallback;
					}
				}
			}
			if (!current_animation)return;
			    current_animation->set_position(position);
			    current_animation->on_update(delta);
		}
}
	void on_input( const ExMessage& msg) {

	}
	void on_render(const camera& camera) {
		if (!current_animation)return;
		// debug: print frame count for current animation to ensure frames exist
		static bool s_printed = false;
		if (!s_printed) {
			int fc = current_animation->get_frame_count();
			std::cerr << "[Player] current animation frame count=" << fc << std::endl;
			s_printed = true;
		}
		current_animation->on_render(camera);
	}
	void set_position(const Vector2& position) {
		this->position = position;
	}
	const Vector2& get_position()const {
		return position;
	}
	void set_target(const Vector2& pos_target) {
		this->target_position = pos_target;
	}
	void set_animation(const std::string& id) {
		if (current_animation_group == &animation_pool[id]) return;
		current_animation_group = &animation_pool[id];
		current_animation_group->left.reset();//重新新动画重第一帧开始
		current_animation_group->right.reset();
		current_animation_group->up.reset();
		current_animation_group->down.reset();
		// ensure we have a valid current animation pointer after switching groups
		if (current_animation_group) {
			// default to down frame if available
			current_animation = &current_animation_group->down;
		}
	}
	void switch_state(const std::string& id) {
		state_machine.switch_to(id);
	}
	Vector2 get_velocity() {
		return velocity;
	}
	StateMachine state_machine;
private:
	bool is_running = false;
    Animation* current_animation = nullptr;
	std::unordered_map<std::string, AnimationGroup>animation_pool;
	facing facing_direction = facing::right;
	Vector2 position;
	Vector2 velocity;
	AnimationGroup* current_animation_group = nullptr;
	Vector2 target_position;

};
#endif