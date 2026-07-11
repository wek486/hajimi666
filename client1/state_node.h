#ifndef STATE_NODE_H
#define STATE_NODE_H
class StateNode {
public:
	StateNode() = default;
	~StateNode() = default;
	virtual void on_enter() {};
	virtual void on_update(float delta) {};
	virtual void on_exit() {};
};











#endif
