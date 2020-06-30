#include "VisualSettings.h"

#include "Particles/ParticlesSystem.h"

const VisualSettings VisualSettings::DEFAULT(
	true,
	20.0f,
	glm::vec3(0.0f, 0.0f, 0.0f)
);

VisualSettings::VisualSettings(bool isAlphaTrailEnabled, float alphaTrailDecay, const glm::vec3& backgroundColor) 
	: m_bAlphaTrail(isAlphaTrailEnabled), m_alphaTrailDecay(alphaTrailDecay), m_bgColor(backgroundColor)
{}

void VisualSettings::apply() {
	if (m_bAlphaTrail)
		EnableAlphaTrail();
	else
		DisableAlphaTrail();
	// TODO apply alphaTrailDecay and backgroundColor
}

void VisualSettings::ImGui() {
	ImGui::Begin("Rendering");
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
	//
	ImGui::End();
}

void VisualSettings::EnableAlphaTrail() {
	SDL_GL_SetSwapInterval(0);
}

void VisualSettings::DisableAlphaTrail() {
	SDL_GL_SetSwapInterval(1);
}