#include "ColorSettings.h"

#include "Particles/ParticlesSystem.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"

ColorSettings::ColorSettings()
	: m_presets("djgColors")
{}

void ColorSettings::ImGui(StateModifier& stateModifier) {
	if (m_bShouldGrabFocus) {
		ImGui::SetWindowFocus();
		m_bShouldGrabFocus = false;
	}
	// Background
	if (ImGui::ColorEdit3("Background Color", (float*)&m_values.bgColor[0])) {
		m_presets.setToPlaceholderSetting();
		applyAndRecord_BackgroundColor(stateModifier);
	}
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
				if (item_current != items[n]) {
					setGradientMode(n == 0);
					applyAndRecord_GradientMode(stateModifier);
				}
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
		if (ImGui::DragFloat("", &m_values.particlesHueStart)) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_HueStart(stateModifier);
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text(" to "); ImGui::SameLine();
		ImGui::PushID(5387655);
		if (ImGui::DragFloat("", &m_values.particlesHueEnd)) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_HueEnd(stateModifier);
		}
		ImGui::PopID();
		ImGui::PopItemWidth();
		//
		if (ImGui::SliderFloat("Saturation", &m_values.particleSaturation, 0, 100)) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_Saturation(stateModifier);
		}
		if (ImGui::SliderFloat("Value", &m_values.particleValue, 0, 100)) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_Value(stateModifier);
		}
	}
	// Color gradient mode
	else {
		ImGui::PushID(6387655);
		if (ImGui::ColorEdit3("", &m_values.particleColorStart[0])) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_ColorStart(stateModifier);
		}
		ImGui::PopID();
		ImGui::PushID(7387655);
		if (ImGui::ColorEdit3("", &m_values.particleColorEnd[0])) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_ColorEnd(stateModifier);
		}
		ImGui::PopID();
	}
	//
	if (m_presets.ImGui(&m_values)) {
		applyAndRecord(stateModifier);
	}
}

bool ColorSettings::ImGuiPresets(StateModifier& stateModifier) {
	if (m_presets.ImGuiDropdown("Colors", &m_values)) {
		applyAndRecord(stateModifier);
		return true;
	}
	return false;
}

void ColorSettings::applyAndRecord(StateModifier& stateModifier) {
	applyAndRecord_BackgroundColor(stateModifier);
	stateModifier.recordChange({ StateChangeType::Color_GradientMode, m_values.bColorModeHueGradient });
	stateModifier.recordChange({ StateChangeType::Color_HueStart,     m_values.particlesHueStart });
	stateModifier.recordChange({ StateChangeType::Color_HueEnd,       m_values.particlesHueEnd });
	stateModifier.recordChange({ StateChangeType::Color_Saturation,   m_values.particleSaturation });
	stateModifier.recordChange({ StateChangeType::Color_Value,        m_values.particleValue });
	stateModifier.recordChange({ StateChangeType::Color_ColorStart,   m_values.particleColorStart });
	stateModifier.recordChange({ StateChangeType::Color_ColorEnd,     m_values.particleColorEnd });
	// Apply only once
	stateModifier.particleSystem().applyParticleColors(m_values);
}

void ColorSettings::applyAndRecord_BackgroundColor(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::Color_Background, m_values.bgColor });
	// There is nothing to do to apply the color, the current value is used every frame
}

void ColorSettings::applyAndRecord_GradientMode(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_GradientMode, m_values.bColorModeHueGradient });
}

void ColorSettings::applyAndRecord_HueStart(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_HueStart, m_values.particlesHueStart });
}

void ColorSettings::applyAndRecord_HueEnd(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_HueEnd, m_values.particlesHueEnd });
}

void ColorSettings::applyAndRecord_Saturation(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_Saturation, m_values.particleSaturation });
}

void ColorSettings::applyAndRecord_Value(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_Value, m_values.particleValue });
}

void ColorSettings::applyAndRecord_ColorStart(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_ColorStart, m_values.particleColorStart });
}

void ColorSettings::applyAndRecord_ColorEnd(StateModifier& stateModifier) {
	stateModifier.particleSystem().applyParticleColors(m_values);
	stateModifier.recordChange({ StateChangeType::Color_ColorEnd, m_values.particleColorEnd });
}