#pragma once

class KeyBindings {
public:
	KeyBindings() = default;
	~KeyBindings() = default;

	bool onKeyPressed(SDL_Scancode scancode);
	void addAction(std::function<void(void)> action);

private:
	std::unordered_map<int, std::function<void(void)>> m_map;
};