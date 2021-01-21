#include "KeyboardLayout.h"

void KeyboardLayout::startSettingKeyboardLayout() {
	m_firstRow.clear();
	m_secondRow.clear();
	m_thirdRow.clear();
	m_bSettingLayout = true;
}

void KeyboardLayout::ImGui() {
	if (ImGui::Button("Set keyboard layout")) {
		startSettingKeyboardLayout();
	}
}

void KeyboardLayout::ImGuiWindow() {
	if (m_bSettingLayout) {
		ImGui::Begin("Enter your keyboard layout");
		ImGui::Text("Please type in all the keys of the first row of your keyboard, in order from left to right.\nThen press ENTER\nThen do the same for the second row\nThen press ENTER\nThen do the same for the third row\nThen press ENTER");
		ImGui::End();
	}
}