#pragma once

struct ActionBinding {
	Action action;
	SDL_Scancode scancode;

	ActionBinding::ActionBinding(Action action, SDL_Scancode scancode)
		: action(action), scancode(scancode)
	{}
	ActionBinding::ActionBinding(Action action)
		: action(action), scancode(SDL_SCANCODE_UNKNOWN)
	{}
};