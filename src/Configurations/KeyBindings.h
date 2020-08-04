#pragma once

enum class ActionType {
	SHAPE,
	SVG_SHAPE,
	LAYOUT,
	STANDALONE,
	TEXT,
	REROLL_RANDOM
};

struct Action {
	std::string name;
	ActionType type;
	size_t index;

	Action(const std::string& name, ActionType type, size_t index = -1)
		: name(name), type(type), index(index)
	{}
	Action() = default;
	~Action() = default;
};

class KeyBindings {
public:
	KeyBindings();
	~KeyBindings() = default;

	Action* getAction(SDL_Scancode scancode);
	void addAction(Action action);
	void setupBindings();
	void ImGui();

private:
	bool isKeyAvailable(SDL_Scancode scancode);
	SDL_Scancode findFirstFromLeft(std::vector<SDL_Scancode> row);
	SDL_Scancode findFirstFromRight(std::vector<SDL_Scancode> row);

	bool ImGui_DrawKey(SDL_Scancode scancode);

private:
	std::unordered_map<int, Action> m_map;
	std::array<std::unordered_map<std::string, Action>, 5> m_allActions;
	size_t nextAvailableKeyIdx = 0;
	std::vector<SDL_Scancode> allKeys;
	std::vector<SDL_Scancode> firstRow = {
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
	std::vector<SDL_Scancode> secondRow = {
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
	std::vector<SDL_Scancode> thirdRow = {
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