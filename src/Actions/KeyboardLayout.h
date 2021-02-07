#pragma once

using GLFW_Keycode = int;

class KeyboardLayout {
public:
	inline const std::vector<GLFW_Keycode>& numbersRow() { return m_numbersRow; }
	inline const std::vector<GLFW_Keycode>& firstRow() { return m_firstRow; }
	inline const std::vector<GLFW_Keycode>& secondRow() { return m_secondRow; }
	inline const std::vector<GLFW_Keycode>& thirdRow() { return m_thirdRow; }

	inline bool isSettingKeyboardLayout() { return m_bSettingLayout; }

	void onKeyUp(GLFW_Keycode keycode);

	void ImGui();
	void ImGuiWindow();

private:
	void startSettingKeyboardLayout();
	std::vector<GLFW_Keycode>& getCurrentRow();

private:
	bool m_bSettingLayout = false;
	int m_settingRowIdx = 0;
	std::vector<GLFW_Keycode> m_numbersRow = {
		SDL_SCANCODE_1,
		SDL_SCANCODE_2,
		SDL_SCANCODE_3,
		SDL_SCANCODE_4,
		SDL_SCANCODE_5,
		SDL_SCANCODE_6,
		SDL_SCANCODE_7,
		SDL_SCANCODE_8,
		SDL_SCANCODE_9,
		SDL_SCANCODE_0,
		SDL_SCANCODE_MINUS,
		SDL_SCANCODE_EQUALS,
	};
	std::vector<GLFW_Keycode> m_firstRow = {
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
	std::vector<GLFW_Keycode> m_secondRow = {
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
	std::vector<GLFW_Keycode> m_thirdRow = {
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

	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(m_numbersRow),
			CEREAL_NVP(m_firstRow),
			CEREAL_NVP(m_secondRow),
			CEREAL_NVP(m_thirdRow)
		);
	}
};