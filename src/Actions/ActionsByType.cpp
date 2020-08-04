#include "ActionsByType.h"

std::unordered_map<std::string, Action>& ActionsByType::operator[](ActionType type) {
	return m_store[(int)type];
}

std::unordered_map<std::string, Action>& ActionsByType::operator[](int type) {
	return m_store[type];
}

ActionsByTypeIterator ActionsByType::begin() {
	return ActionsByTypeIterator(*this);
}

ActionsByTypeIterator ActionsByType::end() {
	return ActionsByTypeIterator(*this, nbActionTypes, m_store[nbActionTypes-1].end());
}

ActionsByTypeIterator::ActionsByTypeIterator(ActionsByType& container)
	: m_container(container), m_index(0), m_mapIt(container[0].begin())
{}

ActionsByTypeIterator::ActionsByTypeIterator(ActionsByType& container, size_t index, std::unordered_map<std::string, Action>::iterator mapIt) 
	: m_container(container), m_index(index), m_mapIt(mapIt)
{}

std::pair<const std::string, Action>& ActionsByTypeIterator::operator*() {
	return *m_mapIt;
}
std::pair<const std::string, Action>* ActionsByTypeIterator::operator->() {
	return m_mapIt.operator->();
}
void ActionsByTypeIterator::operator++() {
	m_mapIt++;
	if (m_mapIt == m_container[m_index].end()) {
		m_index++;
		if (m_index < nbActionTypes)
			m_mapIt = m_container[m_index].begin();
	}
}

bool ActionsByTypeIterator::operator!=(const ActionsByTypeIterator& o) {
	return m_index != o.m_index || m_mapIt != o.m_mapIt;
}