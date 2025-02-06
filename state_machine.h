#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "state_node.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void on_update(float delta);

	//初始状态
	void set_entry(const std::string& id);
	//切换状态机的激活状态
	void switch_to(const std::string& id);
	//状态机中用来注册新的状态
	void register_state(const std::string& id, StateNode* state_node);	

private:
	// 是否初始化
	bool need_init = true;	
	StateNode* current_state = nullptr;
	std::unordered_map<std::string, StateNode*>state_pool;

};

#endif // !_STATE_MACHINE_H_

