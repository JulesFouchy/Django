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
};