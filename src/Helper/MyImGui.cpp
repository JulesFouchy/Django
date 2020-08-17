#include "MyImGui.h"

#include <imgui/imgui_internal.h>

bool MyImGui::SliderFloatWithExtensibleRange(const char* label, float* v, float* vMin, float* vMax, float speed, const char* format, float power) {
	bool b = ImGui::SliderFloat(label, v, *vMin, *vMax, format, power);
	if (ImGui::BeginPopupContextItem("Choose range")){
		ImGui::DragFloat("min", vMin, speed);
		ImGui::DragFloat("max", vMax, speed);
		ImGui::EndPopup();
	}
	return b;
}

void MyImGui::HelpMarker(const char* text) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool MyImGui::AngleWheel(const char* label, float* value_p, float thickness, float radius, int circleNbSegments) {
	ImGuiStyle& style = ImGui::GetStyle();
	float line_height = ImGui::GetTextLineHeight();
	//
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(p.x + radius, p.y + radius);
	// Detect clic
	ImGui::InvisibleButton(label, ImVec2(radius * 2.0f, radius * 2.0f));
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();

	if (is_active) {
		ImVec2 mp = ImGui::GetIO().MousePos;
		*value_p = atan2f(center.y - mp.y, mp.x - center.x);
	}

	float x2 = cosf(*value_p) * radius + center.x;
	float y2 = -sinf(*value_p) * radius + center.y;

	ImU32 col32 = ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	ImU32 col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
	ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddCircleFilled(center, radius, col32, circleNbSegments);
	draw_list->AddLine(center, ImVec2(x2, y2), col32line, thickness);
	draw_list->AddText(ImVec2(p.x + radius * 2.0f + style.ItemInnerSpacing.y, p.y + radius - line_height * 0.5f), col32text, label);

	return is_active;
}

// Thanks to https://github.com/ocornut/imgui/issues/1901
bool MyImGui::Timeline(const char* label, float* timeInSec, float duration) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

	ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 size = ImVec2(250, 15);
    size.x -= style.FramePadding.x * 2;
	// Detect clic
	ImGui::InvisibleButton(label, size);
	const ImVec2& mousePos = ImGui::GetIO().MousePos;
	// Hovered
	bool bHovered = abs(mousePos.x - p.x - size.x * 0.5) < size.x * 0.5
		         && abs(mousePos.y - p.y - size.y * 0.5) < size.y * 0.5;
	// Active
	bool bActive = ImGui::IsItemActive();
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		const ImVec2& mouseClickPos = *ImGui::GetIO().MouseClickedPos;
		bActive |= abs(mouseClickPos.x - p.x - size.x * 0.5) < size.x * 0.5
			    && abs(mouseClickPos.y - p.y - size.y * 0.5) < size.y * 0.5;
	}
	//
	bool bMouseInsideXBounds = abs(mousePos.x - p.x - size.x * 0.5) < size.x * 0.5;

	const float filledWidth = size.x * (*timeInSec)/duration;
	// Draw Progress
	if (bMouseInsideXBounds)
		window->DrawList->AddRectFilled(p, ImVec2(p.x + filledWidth, p.y + size.y), ImGui::GetColorU32(ImGuiCol_FrameBg), 25);
	// Draw Background
    window->DrawList->AddRectFilled(p, ImVec2(p.x + size.x,      p.y + size.y), ImGui::GetColorU32(ImGuiCol_FrameBgActive), 25);
	// Cursor
	if (bHovered || bActive) {
		// Draw line
		if (bMouseInsideXBounds) {
			window->DrawList->AddLine(ImVec2(mousePos.x, p.y), ImVec2(mousePos.x, p.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
		}
		// Show time
		ImGui::BeginTooltip();
		float time = duration * (mousePos.x - p.x) / size.x;
		uint32_t t = (uint32_t)std::min(std::max(time, 0.0f), duration);
		if (duration < 60.0f) {
			ImGui::Text("%us", t);
		}
		else if (duration < 3600.0f) {
			ImGui::Text("%um%02us", t / 60, t % 60);
		}
		else {
			ImGui::Text("%uh%02um%02us", t / 3600, (t % 3600) / 60, t % 60);
		}
		ImGui::EndTooltip();
		// Modify time
		if (bActive) {
			*timeInSec = time;
		}
	}
	return bActive;
}