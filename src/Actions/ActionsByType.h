#pragma once

#include "Action.h"

static constexpr size_t nbActionTypes = 5;

class ActionsByTypeIterator;

class ActionsByType {
	friend class ActionsByTypeIterator;
public:
	std::unordered_map<std::string, Action>& operator[](ActionType type);
	std::unordered_map<std::string, Action>& operator[](int type);
	ActionsByTypeIterator begin();
	ActionsByTypeIterator end();

private:
	std::array<std::unordered_map<std::string, Action>, nbActionTypes> m_store;
};

class ActionsByTypeIterator {
public:
	ActionsByTypeIterator(ActionsByType& container);
	ActionsByTypeIterator(ActionsByType& container, size_t index, std::unordered_map<std::string, Action>::iterator mapIt);
	std::pair<const std::string, Action>& operator*();
	std::pair<const std::string, Action>* operator->();
	void operator++();
	bool operator!=(const ActionsByTypeIterator& o);
private:
	ActionsByType& m_container;
	size_t m_index;
	std::unordered_map<std::string, Action>::iterator m_mapIt;
};