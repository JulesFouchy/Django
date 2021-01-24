#pragma once

namespace Input {
	bool KeyIsDown(SDL_Scancode key);
	bool CtrlOrCmdIsDown();
	glm::vec2 GetMouseInPixels();
	glm::vec2 GetMouseInNormalizedRatioSpace();
	bool IsMouseButtonDown(int button);
}