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
	ImGui::InvisibleButton(label, ImVec2(radius * 2.0f, radius * 2.0f + line_height + style.ItemInnerSpacing.y));
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
	draw_list->AddText(ImVec2(p.x, p.y + radius * 2.0f + style.ItemInnerSpacing.y), col32text, label);

	return is_active;
}