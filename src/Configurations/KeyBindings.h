#pragma once

class Action {
public:
	Action(std::string name, std::function<void(void)> func)
		: m_name(name), m_func(func)
	{}
	Action() = default;
	~Action() = default;

	inline void operator()() { m_func(); }
	inline const std::string& name() { return m_name; }

private:
	std::string m_name;
	std::function<void(void)> m_func;
};

class KeyBindings {
public:
	KeyBindings() = default;
	~KeyBindings() = default;

	bool onKeyPressed(SDL_Scancode scancode);
	void addAction(Action action);

private:
	std::unordered_map<int, Action> m_map;
	size_t nextAvailableKeyIdx = 0;
	std::vector<SDL_Scancode> keys = {
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
		SDL_SCANCODE_A,
		SDL_SCANCODE_S,
		SDL_SCANCODE_D,
		SDL_SCANCODE_F,
		SDL_SCANCODE_G,
		SDL_SCANCODE_H,
		SDL_SCANCODE_J,
		SDL_SCANCODE_K,
		SDL_SCANCODE_L,
		SDL_SCANCODE_Z,
		SDL_SCANCODE_X,
		SDL_SCANCODE_C,
		SDL_SCANCODE_V,
		SDL_SCANCODE_B,
		SDL_SCANCODE_N,
		SDL_SCANCODE_M
	};
};