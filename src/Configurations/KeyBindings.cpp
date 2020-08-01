#include "KeyBindings.h"

bool KeyBindings::onKeyPressed(SDL_Scancode scancode) {
	auto it = m_map.find(scancode);
	if (it != m_map.end()) {
		(it->second)();
		return true;
	}
	return false;
}
void KeyBindings::addAction(std::function<void(void)> action) {
	m_map[SDL_SCANCODE_A] = action;
}