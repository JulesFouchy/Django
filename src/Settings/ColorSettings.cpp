#include "ColorSettings.h"

#include "Particles/ParticlesSystem.h"

ColorSettings::ColorSettings()
	: m_presets("djgColors")
{}

void ColorSettings::apply(ParticlesSystem& partSystem) {
	applyParticlesColors(partSystem);
}

void ColorSettings::ImGui(ParticlesSystem& partSystem) {
	bool b = false;
	bool bColorsChanged = false;
	// Background
	if (ImGui::ColorEdit3("Background Color", (float*)&m_values.bgColor[0]))
		b = true;
	// Particles
	ImGui::Separator();
	ImGui::Text("Particles");
		// Hue
	ImGui::Text("Hue from "); ImGui::SameLine();
	ImGui::PushItemWidth(70);
	ImGui::PushID(4387654);
	if (ImGui::DragFloat("", &m_values.particlesHueStart))
		bColorsChanged = true;
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::Text(" to "); ImGui::SameLine();
	ImGui::PushID(5387655);
	if (ImGui::DragFloat("", &m_values.particlesHueEnd))
		bColorsChanged = true;
	ImGui::PopID();
	ImGui::PopItemWidth();
		//
	if (ImGui::SliderFloat("Saturation", &m_values.particleSaturation, 0, 100))
		bColorsChanged = true;
	if (ImGui::SliderFloat("Value", &m_values.particleValue, 0, 100))
		bColorsChanged = true;
	//
	if (bColorsChanged) {
		b = true;
		applyParticlesColors(partSystem);
	}
	//
	if (m_presets.ImGui(&m_values)) {
		apply(partSystem);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void ColorSettings::applyParticlesColors(ParticlesSystem& partSystem) {
	partSystem.setParticlesColors(m_values);
}