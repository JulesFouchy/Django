#pragma once

class KeyboardLayout {
public:
	inline const std::vector<SDL_Scancode>& firstRow() { return m_firstRow; }
	inline const std::vector<SDL_Scancode>& secondRow() { return m_secondRow; }
	inline const std::vector<SDL_Scancode>& thirdRow() { return m_thirdRow; }

	void onKeyUp(SDL_Scancode scancode);

	void ImGui();
	void ImGuiWindow();

private:
	void startSettingKeyboardLayout();
	std::vector<SDL_Scancode>& getCurrentRow();

private:
	bool m_bSettingLayout = false;
	int m_settingRowIdx = 0;
	std::vector<SDL_Scancode> m_firstRow = {
		SDL_SCANCODE_Q,
		SDL_SCANCODE_W,
		SDL_SCANCODE_E,
		SDL_SCANCODE_R,
		SDL_SCANCODE_T,
		SDL_SCANCODE_Y,
		SDL_SCANCODE_U,
		SDL_SCANCODE_I,
		SDL_SCANCODE_O,
		SDL_SCANCODE_P,
		SDL_SCANCODE_LEFTBRACKET,
		SDL_SCANCODE_RIGHTBRACKET,
		SDL_SCANCODE_BACKSLASH,
	};
	std::vector<SDL_Scancode> m_secondRow = {
		SDL_SCANCODE_A,
		SDL_SCANCODE_S,
		SDL_SCANCODE_D,
		SDL_SCANCODE_F,
		SDL_SCANCODE_G,
		SDL_SCANCODE_H,
		SDL_SCANCODE_J,
		SDL_SCANCODE_K,
		SDL_SCANCODE_L,
		SDL_SCANCODE_SEMICOLON,
		SDL_SCANCODE_APOSTROPHE,
	};
	std::vector<SDL_Scancode> m_thirdRow = {
		SDL_SCANCODE_Z,
		SDL_SCANCODE_X,
		SDL_SCANCODE_C,
		SDL_SCANCODE_V,
		SDL_SCANCODE_B,
		SDL_SCANCODE_N,
		SDL_SCANCODE_M,
		SDL_SCANCODE_COMMA,
		SDL_SCANCODE_PERIOD,
		SDL_SCANCODE_SLASH,
	};
};