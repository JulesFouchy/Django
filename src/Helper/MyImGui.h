#pragma once

namespace MyImGui {
	bool SliderFloatWithExtensibleRange(const char* label, float* v, float* vMin, float* vMax, float speed = 1.0f, const char* format = "%.3f", float power = 1.0f);
	void HelpMarker(const char* text);
	bool AngleWheel(const char* label, float* value_p, float thickness = 2.0f, float radius = 25.0f, int circleNbSegments = 26);
	bool Timeline(const char* label, float* timeInSec, float duration);
}