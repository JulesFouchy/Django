#pragma once

#include "RectSizePosConstrainedRatio.h"

class Viewports {
public:
	static RectSizePos Window; // Size of the window, and it's position inside the screen
	static RectSizePosConstrainedRatio RenderArea; // Size of the main rendering area (excludes the docked ImGui windows), and its position inside the window
	inline static glm::ivec2 SwapYConvention(const glm::ivec2& p) { return { p.x, Window.height() - p.y }; }
};