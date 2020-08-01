#include "KeyBindings.h"

bool KeyBindings::onKeyPressed(SDL_Scancode scancode) {
	auto it = m_map.find(scancode);
	if (it != m_map.end()) {
		(it->second)();
		return true;
	}
	return false;
}
void KeyBindings::addAction(Action action) {
	if (!action.name().compare("s")) {
		m_map[SDL_SCANCODE_W] = action;
	}
	else
		m_map[SDL_SCANCODE_A] = action;
}