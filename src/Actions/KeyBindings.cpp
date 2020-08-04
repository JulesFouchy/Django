#include "KeyBindings.h"

#include <imgui/imgui_internal.h>

static constexpr float keySize = 55.0f;
static constexpr float keyOffsetProp = 0.2f;

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
	m_actionsByType[ActionType_MISCELLANEOUS][textAction.name] = textAction;
	m_actionsByType[ActionType_MISCELLANEOUS][rerollRandomAction.name] = rerollRandomAction;

	for (SDL_Scancode key : firstRow)
		allKeys.push_back(key);
	for (SDL_Scancode key : secondRow)
		allKeys.push_back(key);
	for (SDL_Scancode key : thirdRow)
		allKeys.push_back(key);
}

Action* KeyBindings::getAction(SDL_Scancode scancode) {
	auto it = m_boundActions.find(scancode);
	if (it != m_boundActions.end()) {
		return &it->second;
	}
	return nullptr;
}

void KeyBindings::addAction(Action action) {
	auto& map = m_actionsByType[action.type];
	if (map.find(action.name) != map.end())
		spdlog::warn("There is already an action called '{}' and with the same type ({}) !\nNot adding this one !", action.name, action.type);
	else
		map[action.name] = action;
}

void KeyBindings::setupBindings() {
	// Layout
	for (const auto& kv : m_actionsByType[ActionType::LAYOUT]) {
		SDL_Scancode scancode = findFirstFromLeft(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_boundActions[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' layout because there was no more room on second row !", kv.second.name);
	}
	// Shape
	for (const auto& kv : m_actionsByType[ActionType::SHAPE]) {
		SDL_Scancode scancode = findFirstFromLeft(firstRow);
		if (scancode == SDL_SCANCODE_UNKNOWN)
			scancode = findFirstFromRight(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_boundActions[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' shape because there was no more room on first and second row !", kv.second.name);
	}
	// SVG Shape
	for (const auto& kv : m_actionsByType[ActionType::SVG_SHAPE]) {
		SDL_Scancode scancode = findFirstFromLeft(firstRow);
		if (scancode == SDL_SCANCODE_UNKNOWN)
			scancode = findFirstFromRight(secondRow);
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_boundActions[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' svg-shape because there was no more room on first and second row !", kv.second.name);
	}
	// Layout
	for (const auto& kv : m_actionsByType[ActionType::STANDALONE]) {
		SDL_Scancode scancode = findFirstFromLeft(thirdRow);
		if (scancode == SDL_SCANCODE_UNKNOWN) {
			scancode = findFirstFromRight(secondRow);
		}
		if (scancode != SDL_SCANCODE_UNKNOWN)
			m_boundActions[scancode] = kv.second;
		else
			spdlog::warn("Couldn't give a binding to '{}' standalone because there was no more room on third and second row !", kv.second.name);
	}
	// Text
	if (isKeyAvailable(SDL_SCANCODE_RETURN))
		m_boundActions[SDL_SCANCODE_RETURN] = textAction;
	else
		spdlog::warn("Couldn't give a binding to '{}' because ENTER is already used !", textAction.name);
	// Random reroll
	if (isKeyAvailable(SDL_SCANCODE_SPACE))
		m_boundActions[SDL_SCANCODE_SPACE] = rerollRandomAction;
	else
		spdlog::warn("Couldn't give a binding to '{}' because SPACE is already used !", rerollRandomAction.name);
}

bool KeyBindings::isKeyAvailable(SDL_Scancode scancode) {
	return m_boundActions.find(scancode) == m_boundActions.end();
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
	/*
	static SDL_Scancode selectedScancode = SDL_SCANCODE_UNKNOWN;
	ImGui::Columns(2);
	for (SDL_Scancode key : allKeys) {
		auto it = m_boundActions.find(key);
		if (it != m_boundActions.end()) {
			// Config name
			ImGui::Text(it->second.name.c_str());
			ImGui::NextColumn();
			// Key binding
			if (ImGui::Selectable(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)it->first)), it->first == selectedScancode)) {
				selectedScancode = (SDL_Scancode)it->first;
			}
			ImGui::NextColumn();
		}
	}
	ImGui::Columns(1);
	*/
	ImGui_KeyboardRow(firstRow,  0.0f);
	ImGui_KeyboardRow(secondRow, keyOffsetProp * keySize);
	ImGui_KeyboardRow(thirdRow,  keyOffsetProp * keySize * 2.0f);
}

void KeyBindings::ImGui_KeyboardRow(const std::vector<SDL_Scancode>& row, float indent) {
	ImGui::Indent(indent);
	for (SDL_Scancode scancode : row) {
		ImGui::PushID((int)scancode + 333);
		// Key
		if (ImGui_KeyboardKey(scancode, m_boundActions.find(scancode) != m_boundActions.end()))
			ImGui::OpenPopup("Config list");
		// Config list
		if (ImGui::BeginPopup("Config list")) {
			for (auto it = m_actionsByType.begin(); it != m_actionsByType.end(); ++it) {
				//SDL_Scancode cfgScancode = (SDL_Scancode)it->first;
				const Action& action = it->second;
				if (ImGui::Selectable(action.name.c_str(), false)) {//scancode == cfgScancode)) {
					it->second = action;
				}
			}
			/*for (auto it = m_boundActions.begin(); it != m_boundActions.end(); it++) {
				SDL_Scancode cfgScancode = (SDL_Scancode)it->first;
				const Action& action = it->second;
				if (ImGui::Selectable(action.name.c_str(), scancode == cfgScancode)) {
					it->second = action;
				}
			}*/
			ImGui::EndPopup();
		}
		//
		ImGui::PopID();
		ImGui::SameLine();
	}
	ImGui::NewLine();
	ImGui::Spacing();
}

bool KeyBindings::ImGui_KeyboardKey(SDL_Scancode scancode, bool hasAnActionBound) {
	//
	ImGuiStyle& style = ImGui::GetStyle();
	//
	ImVec2 p = ImGui::GetCursorScreenPos();
	// Detect clic
	ImGui::PushID((int)scancode+1294);
	ImGui::InvisibleButton("", ImVec2(keySize, keySize));
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();

	if (is_active) {
		//ImVec2 mp = ImGui::GetIO().MousePos;
		//*value_p = atan2f(center.y - mp.y, mp.x - center.x);
	}

	ImU32 col32 = ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : hasAnActionBound ? ImGuiCol_Border : ImGuiCol_FrameBg);
	ImU32 col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
	ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float fontSize = 1.5f * draw_list->_Data->FontSize;
	draw_list->AddRectFilled(p, ImVec2(p.x + keySize, p.y + keySize), col32, 10.0f);
	draw_list->AddText(NULL, fontSize, ImVec2(p.x + keySize * 0.5f - fontSize * 0.25f, p.y + keySize * 0.5f - fontSize * 0.45f), col32text, SDL_GetKeyName(SDL_GetKeyFromScancode(scancode)));

	ImGui::PopID();
	return is_active;
}

void KeyBindings::ImGui_ConfigsList(bool open) {
}