#include "VisualSettings.h"

VisualSettings::VisualSettings()
	: m_presets("djgStyle")
{}

void VisualSettings::apply() {
	if (m_values.bAlphaTrail)
		EnableAlphaTrail();
	else
		DisableAlphaTrail();
}

void VisualSettings::ImGui() {
	bool b = false;
	// Alpha trail
		// toggle
	if (ImGui::Checkbox("Alpha Trail", &m_values.bAlphaTrail)) {
		b = true;
		if (m_values.bAlphaTrail)
			EnableAlphaTrail();
		else
			DisableAlphaTrail();
	}
		// strength
	if (ImGui::SliderFloat("Trail Decay", &m_values.alphaTrailDecay, 0.0f, 60.0f))
		b = true;
	// Background Color
	if (ImGui::ColorEdit3("Background Color", (float*)&m_values.bgColor[0]))
		b = true;
	if (m_presets.ImGui(&m_values)) {
		apply();
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void VisualSettings::EnableAlphaTrail() {
	SDL_GL_SetSwapInterval(0);
}
void VisualSettings::DisableAlphaTrail() {
	SDL_GL_SetSwapInterval(1);
}