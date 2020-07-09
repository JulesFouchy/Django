#include "AlphaTrailSettings.h"

AlphaTrailSettings::AlphaTrailSettings()
	: m_presets("djgTrail")
{}

void AlphaTrailSettings::apply() {
	if (m_values.bAlphaTrail)
		EnableAlphaTrail();
	else
		DisableAlphaTrail();
}

void AlphaTrailSettings::ImGui() {
	bool b = false;
	if (ImGui::Checkbox("Enabled", &m_values.bAlphaTrail)) {
		b = true;
		if (m_values.bAlphaTrail)
			EnableAlphaTrail();
		else
			DisableAlphaTrail();
	}
	if (ImGui::SliderFloat("Trail Decay", &m_values.alphaTrailDecay, 0.0f, 60.0f))
		b = true;
	if (m_presets.ImGui(&m_values)) {
		apply();
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void AlphaTrailSettings::EnableAlphaTrail() {
	//SDL_GL_SetSwapInterval(0);
}
void AlphaTrailSettings::DisableAlphaTrail() {
	//SDL_GL_SetSwapInterval(1);
}