#pragma once

namespace MyImGui {
	bool SliderFloatWithExtensibleRange(const char* label, float* v, float* vMin, float* vMax, float speed = 1.0f, const char* format = "%.3f", float power = 1.0f);
	void HelpMarker(const char* text);
	bool AngleWheel(const char* label, float* value_p, float thickness = 2.0f, float radius = 25.0f, int circleNbSegments = 26);
	void TextHourMinSec(float timeInSec, float totalDuration = std::numeric_limits<float>::max());
	bool Timeline(const char* label, float* timeInSec, float duration, float maxWidthInPx = 325.0f);
	void Tooltip(const char* text);
	void ButtonDisabled(const char* label, const char* reasonForDisabling = "Currently disabled");
	bool ButtonWithIcon(unsigned int texID);
	void ButtonWithIconDisabled(unsigned int texID, const char* reasonForDisabling = "Currently disabled");
	void ImageFramed(unsigned int texID, const ImVec2& size, int frameThickness = -1, const ImVec4& frameColor = ImVec4(0, 0, 0, 0), const ImVec4& tintColor = ImVec4(1, 1, 1, 1));
}