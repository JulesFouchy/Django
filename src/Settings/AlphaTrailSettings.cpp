#include "AlphaTrailSettings.h"

AlphaTrailSettings::AlphaTrailSettings()
	: m_presets("djgTrail")
{}

void AlphaTrailSettings::apply() {

}

void AlphaTrailSettings::ImGui() {
	bool b = false;
	if (ImGui::Checkbox("Enabled", &m_values.bAlphaTrail))
		b = true;
	if (ImGui::SliderFloat("Trail Decay", &m_values.alphaTrailDecay, 0.0f, 60.0f))
		b = true;
	if (m_presets.ImGui(&m_values)) {
		apply();
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}