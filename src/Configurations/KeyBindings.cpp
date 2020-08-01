#include "KeyBindings.h"

bool KeyBindings::onKeyPressed(SDL_Scancode scancode) {
	spdlog::info((int)scancode);
	auto it = m_map.find(scancode);
	if (it != m_map.end()) {
		(it->second)();
		return true;
	}
	return false;
}
void KeyBindings::addAction(Action action) {
	if (nextAvailableKeyIdx<26)
	m_map[keys[nextAvailableKeyIdx++]] = action;
}