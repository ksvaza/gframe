#pragma once

#include <memory>
#include <stack>

#include "State.hpp"

class StateMachine
{
public:
	StateMachine() {};
	~StateMachine() {};
	void AddState(std::unique_ptr<State>  newState, bool isReplacing);
	void RemoveState();
	void ProcessStateChanges();
	std::unique_ptr<State>& GetActiveState();
private:
	std::stack<std::unique_ptr<State>> _states;
	std::unique_ptr<State>  _newState;
	bool _isRemoving, _isAdding, _isreplacing;
};