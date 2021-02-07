#pragma once

using GLFW_Keycode = int;

struct ActionBinding {
	Action action;
	GLFW_Keycode keycode;

	ActionBinding(Action action, GLFW_Keycode keycode)
		: action(action), keycode(keycode)
	{}
	ActionBinding(Action action)
		: action(action), keycode(GLFW_KEY_UNKNOWN)
	{}
};
