#include "KeyBindings.h"

Action* KeyBindings::getAction(SDL_Scancode scancode) {
	auto it = m_map.find(scancode);
	if (it != m_map.end()) {
		return &it->second;
	}
	return nullptr;
}
void KeyBindings::addAction(Action action) {
	SDL_Scancode scancode;
	switch (action.type) {
	case ActionType::TEXT:
		m_map[SDL_SCANCODE_RETURN] = action;
		break;
	case ActionType::REROLL_RANDOM:
		m_map[SDL_SCANCODE_SPACE] = action;
		break;
	case ActionType::LAYOUT:
		scancode = findFirstFromLeft(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_map[scancode] = action;
		break;
	case ActionType::SHAPE:
	case ActionType::SVG_SHAPE:
		scancode = findFirstFromLeft(firstRow);
		if (scancode == SDL_SCANCODE_UNKNOWN) {
			scancode = findFirstFromRight(secondRow);
		}
		if (scancode != SDL_SCANCODE_UNKNOWN) {
			m_map[scancode] = action;
		}
		break;
	case ActionType::STANDALONE:
		scancode = findFirstFromLeft(thirdRow);
		if (scancode == SDL_SCANCODE_UNKNOWN) {
			scancode = findFirstFromRight(secondRow);
		}
		if (scancode != SDL_SCANCODE_UNKNOWN) {
			m_map[scancode] = action;
		}
		break;
	default:
		assert(false && "Unsupported ActionType !");
		break;
	}
}

bool KeyBindings::isKeyAvailable(SDL_Scancode scancode) {
	return m_map.find(scancode) == m_map.end();
}

SDL_Scancode KeyBindings::findFirstFromLeft(std::vector<SDL_Scancode> row) {
	for (size_t i = 0; i < row.size(); ++i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return SDL_SCANCODE_UNKNOWN;
}

SDL_Scancode KeyBindings::findFirstFromRight(std::vector<SDL_Scancode> row) {
	for (size_t i = row.size() - 1; i >= 0; --i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return SDL_SCANCODE_UNKNOWN;
}