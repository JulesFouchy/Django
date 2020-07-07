#include "ColorSettings.h"

ColorSettings::ColorSettings()
	: m_presets("djgColors")
{}

void ColorSettings::apply() {
	applyParticlesColors();
}

void ColorSettings::ImGui() {
	bool b = false;
	// Background
	if (ImGui::ColorEdit3("Background Color", (float*)&m_values.bgColor[0]))
		b = true;
	// Particles
	ImGui::Text("Hue from "); ImGui::SameLine();
	ImGui::PushItemWidth(70);
	ImGui::PushID(4387654);
	if (ImGui::DragFloat("", &m_values.particlesHueStart)) {
		b = true;
		applyParticlesColors();
	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::Text(" to "); ImGui::SameLine();
	ImGui::PushID(5387655);
	if (ImGui::DragFloat("", &m_values.particlesHueEnd)) {
		b = true;
		applyParticlesColors();
	}
	ImGui::PopID();
	ImGui::PopItemWidth();
	//
	if (m_presets.ImGui(&m_values)) {
		apply();
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void ColorSettings::applyParticlesColors() {

}