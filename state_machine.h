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

	//��ʼ״̬
	void set_entry(const std::string& id);
	//�л�״̬���ļ���״̬
	void switch_to(const std::string& id);
	//״̬��������ע���µ�״̬
	void register_state(const std::string& id, StateNode* state_node);	

private:
	// �Ƿ��ʼ��
	bool need_init = true;	
	StateNode* current_state = nullptr;
	std::unordered_map<std::string, StateNode*>state_pool;

};

#endif // !_STATE_MACHINE_H_

