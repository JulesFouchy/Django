#include "AlphaTrailSettings.h"

#include "Helper/MyImGui.h"
#include "Renderer.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"

AlphaTrailSettings::AlphaTrailSettings()
	: m_presets("djgTrail")
{}

void AlphaTrailSettings::apply(StateModifier& stateModifier) {
	if (m_values.bEnabled) {
		if (!m_values.bFixResiduals)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	stateModifier.recordChange({ StateChangeType::AlphaTrail, m_values });
}

void AlphaTrailSettings::ImGui(const glm::vec3& bgColor, StateModifier& stateModifier) {
	bool b = false;
	if (ImGui::Checkbox("Enabled", &m_values.bEnabled)) {
		b = true;
		if (m_values.bEnabled)
			stateModifier.renderer().clearRenderBuffer(bgColor);
		apply(stateModifier);
	}
	if (m_values.bEnabled) {
		if (ImGui::SliderFloat("Trail Decay", &m_values.decay, 0.0f, 30.0f))
			b = true;
		if (ImGui::Checkbox("Fix Residuals", &m_values.bFixResiduals)) {
			b = true;
			apply(stateModifier);
		}
		ImGui::SameLine();
		MyImGui::HelpMarker("For very small values of Trail Decay, some artifacts appear.\nCheck this only if you see them, since the fix comes at a small performance cost.");
		if (m_values.bFixResiduals) {
			if (ImGui::SliderFloat("Threshold", &m_values.threshold, 0.0f, 0.5f))
				b = true;
			if (ImGui::SliderFloat("Min Alpha", &m_values.minAlpha, 0.0f, 0.2f))
				b = true;
		}
	}
	bool prevBTrail = m_values.bEnabled;
	if (m_presets.ImGui(&m_values)) {
		if (m_values.bEnabled && !prevBTrail)
			stateModifier.renderer().clearRenderBuffer(bgColor);
		apply(stateModifier);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}