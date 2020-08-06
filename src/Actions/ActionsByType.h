#pragma once

#include "Action.h"

static constexpr size_t nbActionTypes = 5;
constexpr int ActionType_MISCELLANEOUS = 4;

class ActionsByTypeIterator;

struct ActionBinding {
	Action action;
	SDL_Scancode scancode;

	ActionBinding::ActionBinding(Action action, SDL_Scancode scancode)
		: action(action), scancode(scancode)
	{}	
	ActionBinding::ActionBinding(Action action)
		: action(action), scancode(SDL_SCANCODE_UNKNOWN)
	{}
};

class ActionsByType {
	friend class ActionsByTypeIterator;
public:
	std::unordered_map<std::string, ActionBinding*>& operator[](ActionType type);
	std::unordered_map<std::string, ActionBinding*>& operator[](int type);
	ActionsByTypeIterator begin();
	ActionsByTypeIterator end();

	ActionBinding* tryFind(const std::string& name, ActionType type);

private:
	std::array<std::unordered_map<std::string, ActionBinding*>, nbActionTypes> m_store;
};

class ActionsByTypeIterator {
public:
	ActionsByTypeIterator(ActionsByType& container);
	ActionsByTypeIterator(ActionsByType& container, size_t index, std::unordered_map<std::string, ActionBinding*>::iterator mapIt);
	std::pair<const std::string, ActionBinding*>& operator*();
	std::pair<const std::string, ActionBinding*>* operator->();
	void operator++();
	bool operator!=(const ActionsByTypeIterator& o);
private:
	ActionsByType& m_container;
	size_t m_index;
	std::unordered_map<std::string, ActionBinding*>::iterator m_mapIt;
};