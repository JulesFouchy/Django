#pragma once

namespace MyImGui {
	bool SliderFloatWithExtensibleRange(const char* label, float* v, float* vMin, float* vMax, float speed = 1.0f, const char* format = "%.3f", float power = 1.0f);
	void HelpMarker(const char* text);
	bool AngleWheel(const char* label, float* value_p, float thickness = 2.0f, float radius = 25.0f, int circleNbSegments = 26);
	void TimeFormatedHMS(float timeInSec, float totalDuration = 0.0f);
	bool Timeline(const char* stringID, float* timeInSec, float duration, bool bShowCurrentTime = true, float maxWidthInPx = 325.0f);
	void Tooltip(const char* text);
	void ButtonDisabled(const char* label, const char* reasonForDisabling = "Currently disabled");
	bool ButtonWithIcon(unsigned int texID, const ImVec4& tintColor = ImVec4(1, 1, 1, 1), const ImVec4& backgroundColor = ImVec4(0, 0, 0, 1), int frameThickness = 2, int widthInPixels = 24, int heightInPixels = 24);
	void ButtonWithIconDisabled(unsigned int texID, const char* reasonForDisabling = "Currently disabled", int frameThickness = 2, int widthInPixels = 24, int heightInPixels = 24);
	void ImageFramed(unsigned int texID, const ImVec2& size, int frameThickness = -1, const ImVec4& frameColor = ImVec4(0, 0, 0, 0), const ImVec4& backgroundColor = ImVec4(0, 0, 0, 1), const ImVec4& tintColor = ImVec4(1, 1, 1, 1));
	bool InputUInt(const char* label, unsigned int* value_p);
	bool BeginPopupContextMenuFromButton(const char* label, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None);
}