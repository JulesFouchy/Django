#include "KeyboardLayout.h"

void KeyboardLayout::startSettingKeyboardLayout() {
	m_firstRow.clear();
	m_secondRow.clear();
	m_thirdRow.clear();
	m_bSettingLayout = true;
	m_settingRowIdx = 0;
}

void KeyboardLayout::onKeyUp(SDL_Scancode scancode) {
	if (m_bSettingLayout) {
		if (scancode == SDL_SCANCODE_RETURN) {
			m_settingRowIdx++;
			if (m_settingRowIdx == 3) {
				m_bSettingLayout = false;
			}
		}
		else if (scancode == SDL_SCANCODE_BACKSPACE) {
			if (getCurrentRow().size() > 0)
				getCurrentRow().pop_back();
		}
		else {
			getCurrentRow().push_back(scancode);
		}
	}
}

std::vector<SDL_Scancode>& KeyboardLayout::getCurrentRow() {
	switch (m_settingRowIdx) {
	case 0:
		return m_firstRow;
	case 1:
		return m_secondRow;
	case 2:
		return m_thirdRow;
	default:
		spdlog::error("[KeyboardLayout::getCurrentRow] Invalid value for m_firstRow");
		return m_firstRow;
	}
}

void KeyboardLayout::ImGui() {
	if (ImGui::Button("Configure your Keyboard Layout")) {
		startSettingKeyboardLayout();
	}
}

void KeyboardLayout::ImGuiWindow() {
	if (m_bSettingLayout) {
		ImGui::Begin("Enter your keyboard layout");
		ImGui::Text("Please type in all the keys of the first row of your keyboard, in order from left to right (Q, W, E, ...)\nThen press ENTER\nThen do the same for the second row\nThen press ENTER\nThen do the same for the third row\nThen press ENTER\n\nNB : you can press backspace to delete the key you just typed in");
		ImGui::End();
	}
}