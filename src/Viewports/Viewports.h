#pragma once

#include "RectSizePos.h"

class Viewports {
public:
	static RectSize Window; // Size of the window
	static RectSizePos RenderArea; // Size of the main rendering area (excludes the docked ImGui windows), and its position inside the window
};