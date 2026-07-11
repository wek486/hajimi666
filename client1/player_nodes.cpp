#include"player_state_node.h"
#include"player.h"
extern int id_player;
extern Player* player1;
extern Player* player2;

void PlayerRunState::on_enter() {
	
    player1->set_animation("hajimi_run");
    // debug: print frame counts for run animations (safe check)
    {
        Atlas* a = ResourcesManager::instance()->find_atlas("hajimi_run_left");
        if (!a) std::cerr << "[PlayerRunState] hajimi_run_left atlas not found" << std::endl;
    }
    player2->set_animation("manbo_run");
	
	
	//播放跑步动画
}
void PlayerRunState::on_update(float delta) {
	//更新跑步动画
	if (id_player == 1) {
		if (player1->get_velocity().length() < 0.1f) {
			player1->switch_state("idle");
		}
	}
	else {
		if (player2->get_velocity().length() <0.1f) {
			player2->switch_state("idle");
		}
	}
}

void PlayerIdleState::on_enter() {
	//播放待机动画
	
		player1->set_animation("hajimi_idle");

	
		player2->set_animation("manbo_idle");
	//不能用判断来决定图集,因为在客服端两个玩家都要渲染
}
void PlayerIdleState::on_update(float delta) {
	if (id_player == 1) {
		if (player1->get_velocity().length() >0.1f) {
			player1->switch_state("run");
		}
	}
	else {
		if (player2->get_velocity().length() > 0.1f) {
			player2->switch_state("run");
		}
	}
	//更新待机动画
}
//循环爆发：

//状态机一初始化（idle），检测到速度为 0，立刻 switch_to("run")。

//到了下一帧，游戏进入 PlayerRunState::on_update，检测到速度依然为 0，立刻又 switch_to("idle")。

//这种“帧内无限互相切状态”的行为导致 Player::set_animation() 里的.reset() 不断被调用，主线程极度高负载或者陷入状态机内部的递归 / 死循环，整个主循环被拖死或卡住，导致 timer_countdown 没有机会再往下消费时间！
