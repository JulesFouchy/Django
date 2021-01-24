#include "AlphaTrailSettings.h"

#include "Helper/MyImGui.h"
#include "Renderer.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"
#include "Settings/SettingsManager.h"

AlphaTrailSettings::AlphaTrailSettings()
	: m_presets("djgTrail")
{}

void AlphaTrailSettings::ImGui(StateModifier& stateModifier) {
	if (m_bShouldGrabFocus) {
		ImGui::SetWindowFocus();
		m_bShouldGrabFocus = false;
	}
	bool bEnabled = m_values.bEnabled;
	if (ImGui::Checkbox("Enabled", &bEnabled)) {
		setApplyAndRecord_Enabled(bEnabled, stateModifier);
	}
	if (m_values.bEnabled) {
		if (ImGui::SliderFloat("Trail Decay", &m_values.decay, 0.0f, 30.0f)) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_Decay(stateModifier);
		}
		if (ImGui::Checkbox("Fix Residuals", &m_values.bFixResiduals)) {
			m_presets.setToPlaceholderSetting();
			applyAndRecord_FixResiduals(stateModifier);
		}
		ImGui::SameLine();
		MyImGui::HelpMarker("For very small values of Trail Decay, some artifacts appear.\nCheck this only if you see them, since the fix comes at a small performance cost.");
		if (m_values.bFixResiduals) {
			if (ImGui::SliderFloat("Threshold", &m_values.threshold, 0.0f, 0.5f)) {
				m_presets.setToPlaceholderSetting();
				applyAndRecord_Threshold(stateModifier);
			}
			if (ImGui::SliderFloat("Min Alpha", &m_values.minAlpha, 0.0f, 0.2f)) {
				m_presets.setToPlaceholderSetting();
				applyAndRecord_MinAlpha(stateModifier);
			}
		}
	}
	bool bPrevEnabled = m_values.bEnabled;
	if (m_presets.ImGui(&m_values)) {
		bool bEnabled = m_values.bEnabled;
		m_values.bEnabled = bPrevEnabled;
		setEnabled_ApplyAndRecordAll(bEnabled, stateModifier);
	}
}

bool AlphaTrailSettings::ImGuiPresets(StateModifier& stateModifier) {
	bool bPrevEnabled = m_values.bEnabled;
	if (m_presets.ImGuiDropdown("Trail", &m_values)) {
		bool bEnabled = m_values.bEnabled;
		m_values.bEnabled = bPrevEnabled;
		setEnabled_ApplyAndRecordAll(bEnabled, stateModifier);
		return true;
	}
	return false;
}

void AlphaTrailSettings::applyGLBlendState() {
	if (m_values.bEnabled && !m_values.bFixResiduals)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void AlphaTrailSettings::setEnabled_ApplyAndRecordAll(bool bEnabled, StateModifier& stateModifier) {
	setApplyAndRecord_Enabled(bEnabled, stateModifier, false);
	stateModifier.recordChange({ StateChangeType::AlphaTrail_FixResiduals, m_values.bFixResiduals });
	applyAndRecord_Decay(stateModifier);
	applyAndRecord_Threshold(stateModifier);
	applyAndRecord_MinAlpha(stateModifier);
	applyAndRecord_PresetName(stateModifier);
}

void AlphaTrailSettings::setApplyAndRecord_Enabled(bool bEnabled, StateModifier& stateModifier, bool bCanSetToPlaceholderSetting) {
	// Clear if we are transitionning from no trail to trail
	if (!m_values.bEnabled && bEnabled)
		stateModifier.renderer().clearRenderBuffer(stateModifier.settingsManager().get().colors().backgroundColor());
	// Set
	if (m_values.bEnabled != bEnabled) {
		m_values.bEnabled = bEnabled;
		if (bCanSetToPlaceholderSetting)
			m_presets.setToPlaceholderSetting();
	}
	// Apply and record
	applyGLBlendState();
	stateModifier.recordChange({StateChangeType::AlphaTrail_Enabled, m_values.bEnabled});
}

void AlphaTrailSettings::applyAndRecord_FixResiduals(StateModifier& stateModifier) {
	applyGLBlendState();
	stateModifier.recordChange({ StateChangeType::AlphaTrail_FixResiduals, m_values.bFixResiduals });
}

void AlphaTrailSettings::applyAndRecord_Decay(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::AlphaTrail_Decay, m_values.decay });
	// There is nothing to do to apply, the current value is used every frame
}

void AlphaTrailSettings::applyAndRecord_Threshold(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::AlphaTrail_Threshold, m_values.threshold });
	// There is nothing to do to apply, the current value is used every frame
}

void AlphaTrailSettings::applyAndRecord_MinAlpha(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::AlphaTrail_MinAlpha, m_values.minAlpha});
	// There is nothing to do to apply, the current value is used every frame
}

void AlphaTrailSettings::applyAndRecord_PresetName(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::AlphaTrail_PresetName, m_presets.getPresetName() });
}