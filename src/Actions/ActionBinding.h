#pragma once

struct ActionBinding {
	Action action;
	SDL_Scancode scancode;

	ActionBinding(Action action, SDL_Scancode scancode)
		: action(action), scancode(scancode)
	{}
	ActionBinding(Action action)
		: action(action), scancode(SDL_SCANCODE_UNKNOWN)
	{}
};
