#include "MyImGui.h"
#include <imgui/imgui.h>

bool MyImGui::SliderFloatWithExtensibleRange(const char* label, float* v, float* vMin, float* vMax, float speed, const char* format, float power) {
	bool b = ImGui::SliderFloat(label, v, *vMin, *vMax, format, power);
	if (ImGui::BeginPopupContextItem("Choose range")){
		ImGui::DragFloat("min", vMin, speed);
		ImGui::DragFloat("max", vMax, speed);
		ImGui::EndPopup();
	}
	return b;
}