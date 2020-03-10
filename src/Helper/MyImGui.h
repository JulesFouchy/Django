#pragma once

namespace MyImGui {
	bool SliderFloatWithExtensibleRange(const char* label, float* v, float* vMin, float* vMax, float speed = 1.0f, const char* format = "%.3f", float power = 1.0f);
}