#include "KeyboardLayout.h"

#include "Helper/MyImGui.h"

void KeyboardLayout::startSettingKeyboardLayout() {
	m_numbersRow.clear();
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
			if (m_settingRowIdx == 4) {
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
		return m_numbersRow;
	case 1:
		return m_firstRow;
	case 2:
		return m_secondRow;
	case 3:
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
	if (LiveMode::ShowHelpMarkers()) {
		ImGui::SameLine();
		MyImGui::HelpMarker(R"V0G0N(The virtual keyboard you see above might not match your keyboard perfectly.
To fix this, just press the button and you will be presented with instructions.
)V0G0N");
	}
}

void KeyboardLayout::ImGuiWindow() {
	if (m_bSettingLayout) {
		ImGui::Begin("Enter your keyboard layout");
		ImGui::Text("Please type in all the keys of the first row of your keyboard (the one with the numbers), in order from left to right\nThen press ENTER\nThen do the same for the second row\nThen press ENTER\nThen do the same for the third row\nThen press ENTER\nThen do the same for the fourth row\nThen press ENTER\n\nNB : you can press backspace to delete the key you just typed in");
		ImGui::End();
	}
}