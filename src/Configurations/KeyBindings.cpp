#include "KeyBindings.h"

constexpr int ActionType_MISCELLANEOUS = 4;
static const Action textAction(
	"Text",
	ActionType::TEXT
);
static const Action rerollRandomAction(
	"Reroll random",
	ActionType::REROLL_RANDOM
);

KeyBindings::KeyBindings() {
	m_allActions[ActionType_MISCELLANEOUS][textAction.name] = textAction;
	m_allActions[ActionType_MISCELLANEOUS][rerollRandomAction.name] = rerollRandomAction;
}

Action* KeyBindings::getAction(SDL_Scancode scancode) {
	auto it = m_map.find(scancode);
	if (it != m_map.end()) {
		return &it->second;
	}
	return nullptr;
}

void KeyBindings::addAction(Action action) {
	auto& map = m_allActions[(int)action.type];
	if (map.find(action.name) != map.end())
		spdlog::warn("There is already an action called '{}' and with the same type ({}) !\nNot adding this one !", action.name, action.type);
	else
		map[action.name] = action;
}

void KeyBindings::setupBindings() {
	// Layout
	for (const auto& kv : m_allActions[(int)ActionType::LAYOUT]) {
		SDL_Scancode scancode = findFirstFromLeft(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_map[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' layout because there was no more room on second row !", kv.second.name);
	}
	// Shape
	for (const auto& kv : m_allActions[(int)ActionType::SHAPE]) {
		SDL_Scancode scancode = findFirstFromLeft(firstRow);
		if (scancode == SDL_SCANCODE_UNKNOWN)
			scancode = findFirstFromRight(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_map[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' shape because there was no more room on first and second row !", kv.second.name);
	}
	// SVG Shape
	for (const auto& kv : m_allActions[(int)ActionType::SVG_SHAPE]) {
		SDL_Scancode scancode = findFirstFromLeft(firstRow);
		if (scancode == SDL_SCANCODE_UNKNOWN)
			scancode = findFirstFromRight(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_map[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' svg-shape because there was no more room on first and second row !", kv.second.name);
	}
	// Layout
	for (const auto& kv : m_allActions[(int)ActionType::STANDALONE]) {
		SDL_Scancode scancode = findFirstFromLeft(thirdRow);
		if (scancode == SDL_SCANCODE_UNKNOWN) {
			scancode = findFirstFromRight(secondRow);
		}
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_map[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' standalone because there was no more room on third and second row !", kv.second.name);
	}
	// Text
	if (isKeyAvailable(SDL_SCANCODE_RETURN))
		m_map[SDL_SCANCODE_RETURN] = textAction;
	else
		spdlog::warn("Couldn't give a binding to '{}' because ENTER is already used !", textAction.name);
	// Random reroll
	if (isKeyAvailable(SDL_SCANCODE_SPACE))
		m_map[SDL_SCANCODE_SPACE] = rerollRandomAction;
	else
		spdlog::warn("Couldn't give a binding to '{}' because SPACE is already used !", rerollRandomAction.name);
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
	for (int i = row.size() - 1; i >= 0; --i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return SDL_SCANCODE_UNKNOWN;
}

void KeyBindings::ImGui() {
	static SDL_Scancode selectedScancode = SDL_SCANCODE_UNKNOWN;
	ImGui::Columns(2);
	for (auto& kv : m_map) {
		// Config name
		ImGui::Text(kv.second.name.c_str());
		ImGui::NextColumn();
		// Key binding
		if (ImGui::Selectable(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)kv.first)), kv.first == selectedScancode)) {
			selectedScancode = (SDL_Scancode)kv.first;
		}
		ImGui::NextColumn();
	}
	ImGui::Columns(1);
}