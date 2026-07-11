/*函数“void on_update(float)”已有主体
“PlayerRunState” : “class”类型重定义
“PlayerIdleState” : “class”类型重定义
使用了未定义类型“PlayerIdleState”
“StateMachine::register_state” : 函数不接受 1 个参数
使用了未定义类型“PlayerRunState”
“StateMachine::register_state” : 函数不接受 1 个参数
使用了未定义类型“PlayerRunState”
使用了未定义类型“PlayerRunState”
使用了未定义类型“PlayerIdleState”
函数“void on_enter(void)”已有主体
使用了未定义类型“PlayerIdleState”*/
//这是没头文件宏定义保护导致的
#pragma once
#include"state_node.h"
class PlayerRunState :public StateNode {
public:
	PlayerRunState() = default;
	~PlayerRunState() = default;
	void on_enter()override;
	void on_update(float delta)override;
	
};
class PlayerIdleState :public StateNode {
public:
	PlayerIdleState()=default;
	~PlayerIdleState() = default;
	void on_enter()override;
	void on_update(float delta)override;
};