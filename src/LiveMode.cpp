#include "LiveMode.h"

bool LiveMode::s_on = false;

void LiveMode::ImGui() {
	ImGui::Checkbox("Live Mode", &s_on);
}