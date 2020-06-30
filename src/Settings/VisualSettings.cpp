#include "VisualSettings.h"

void VisualSettings::apply() {
	if (m_bAlphaTrail)
		EnableAlphaTrail();
	else
		DisableAlphaTrail();
}

void VisualSettings::ImGui() {
	// Alpha trail
		// toggle
	if (ImGui::Checkbox("Alpha Trail", &m_bAlphaTrail)) {
		if (m_bAlphaTrail)
			EnableAlphaTrail();
		else
			DisableAlphaTrail();
	}
		// strength
	ImGui::SliderFloat("Trail Decay", &m_alphaTrailDecay, 0.0f, 60.0f);
	// Background Color
	ImGui::ColorEdit3("Background Color", (float*)&m_bgColor[0]);
}

void VisualSettings::EnableAlphaTrail() {
	SDL_GL_SetSwapInterval(0);
}
void VisualSettings::DisableAlphaTrail() {
	SDL_GL_SetSwapInterval(1);
}