#include "ColorSettings.h"

ColorSettings::ColorSettings()
	: m_presets("djgColors")
{}

void ColorSettings::apply() {

}

void ColorSettings::ImGui() {
	bool b = false;
	// Background Color
	if (ImGui::ColorEdit3("Background Color", (float*)&m_values.bgColor[0]))
		b = true;
	if (m_presets.ImGui(&m_values)) {
		apply();
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}