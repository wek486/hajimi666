#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "state_node.h"
#include <string>
#include<unordered_map>

class StateMachine {
public:
	StateMachine();
	~StateMachine() = default;
	void on_update(float delta);
	void set_entry(const std::string& id);
	void switch_to(const std::string& id);
	void register_state(const std::string& id, StateNode* state_node);//注册新的状态
private:
	bool need_init = true;//是否初始化过
	StateNode* current_state = NULL;
	std::unordered_map<std::string, StateNode*>state_pool;


};


#endif