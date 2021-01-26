#include "LiveMode.h"
#include "Helper/MyImGui.h"

bool LiveMode::s_on = false;
bool LiveMode::s_hideHelpMarkers = false;

void LiveMode::ImGui() {
	ImGui::Checkbox("Live Mode", &s_on);
	if (LiveMode::ShowHelpMarkers()) {
		ImGui::SameLine();
		MyImGui::HelpMarker(R"V0G0N(In Live Mode some things are hidden to let you focus on the essential. In Live Mode you can't change your shortcuts, create new presets and so on...
)V0G0N");
	}
	ImGui::Checkbox("Hide Help markers", &s_hideHelpMarkers);
}