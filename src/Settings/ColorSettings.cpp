#include "ColorSettings.h"

#include "Particles/ParticlesSystem.h"

ColorSettings::ColorSettings()
	: m_presets("djgColors")
{}

void ColorSettings::apply(StateModifier& stateModifier) {
	//stateModifier.setParticleColors();
}

void ColorSettings::ImGui(StateModifier& stateModifier) {
	bool b = false;
	bool bColorsChanged = false;
	// Background
	if (ImGui::ColorEdit3("Background Color", (float*)&m_values.bgColor[0]))
		b = true;
	// Particles
		// Choose mode
	const char* items[] = { "Hue gradient", "Color Gradient" };
	const char* item_current = items[m_values.bColorModeHueGradient ? 0 : 1];
	if (ImGui::BeginCombo("Gradient mode", item_current, 0))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (item_current == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				if (item_current != items[n])
					bColorsChanged = true;
				m_values.bColorModeHueGradient = n == 0;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	// Hue gradient mode
	if (m_values.bColorModeHueGradient) {
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
	}
	// Color gradient mode
	else {
		ImGui::PushID(6387655);
		if (ImGui::ColorEdit3("", &m_values.particleColorStart[0]))
			bColorsChanged = true;
		ImGui::PopID();
		ImGui::PushID(7387655);
		if (ImGui::ColorEdit3("", &m_values.particleColorEnd[0]))
			bColorsChanged = true;
		ImGui::PopID();
	}
	//
	if (bColorsChanged) {
		b = true;
		apply(stateModifier);
	}
	//
	if (m_presets.ImGui(&m_values)) {
		apply(stateModifier);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}