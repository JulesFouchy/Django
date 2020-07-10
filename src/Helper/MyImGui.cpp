#include "MyImGui.h"

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