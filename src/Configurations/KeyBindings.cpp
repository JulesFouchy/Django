#include "KeyBindings.h"

Action* KeyBindings::getAction(SDL_Scancode scancode) {
	auto it = m_map.find(scancode);
	if (it != m_map.end()) {
		return &it->second;
	}
	return nullptr;
}
void KeyBindings::addAction(Action action) {
	if (nextAvailableKeyIdx<26)
	m_map[keys[nextAvailableKeyIdx++]] = action;
}