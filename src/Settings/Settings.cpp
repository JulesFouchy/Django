#include "Settings.h"

#include "StateModifier.h"

#include <cereal/archives/json.hpp>
#include <fstream>

void Settings::ImGuiWindows(StateModifier& stateModifier) {
	if (m_bOpenParticles) {
		ImGui::Begin("Particles");
		m_particleSystemSettings.ImGui(stateModifier);
		ImGui::End();
	}
	if (m_bOpenColors) {
		ImGui::Begin("Colors");
		m_colorSettings.ImGui(stateModifier);
		ImGui::End();
	}
	if (m_bOpenTrail) {
		ImGui::Begin("Trail");
		m_alphaTrailSettings.ImGui(stateModifier);
		ImGui::End();
	}
	if (m_bOpenWind) {
		ImGui::Begin("Wind");
		m_windSettings.ImGui(stateModifier);
		ImGui::End();
	}
	if (m_bOpenPhysics) {
		ImGui::Begin("Physics");
		m_physicsSettings.ImGui(stateModifier);
		ImGui::End();
	}
}

void Settings::ImGuiMainWindow(StateModifier& stateModifier) {
	ImGui::Begin("Presets");
	m_physicsSettings.ImGuiPresets(stateModifier);
	m_windSettings.ImGuiPresets(stateModifier);
	m_alphaTrailSettings.ImGuiPresets(stateModifier);
	m_colorSettings.ImGuiPresets(stateModifier);
	m_particleSystemSettings.ImGuiPresets(stateModifier);
	ImGui::End();
}

void Settings::ImGuiOpenWindowsCheckboxes() {
	ImGui::Checkbox("Trail",     &m_bOpenTrail);
	ImGui::Checkbox("Colors",    &m_bOpenColors);
	ImGui::Checkbox("Wind",      &m_bOpenWind);
	ImGui::Checkbox("Physics",   &m_bOpenPhysics);
	ImGui::Checkbox("Particles", &m_bOpenParticles);
}

void Settings::applyAndRecord(StateModifier& stateModifier) {
	m_windSettings.applyAndRecord(stateModifier); // must be before particleSystemSettings
	m_particleSystemSettings.applyAndRecord(stateModifier);
	m_alphaTrailSettings.setEnabled_ApplyAndRecordAll(m_alphaTrailSettings.getValues().bEnabled, stateModifier);
	m_colorSettings.applyAndRecord(stateModifier); // must be after particleSystemSettings
	m_physicsSettings.applyAndRecord(stateModifier);
}

void Settings::serializeTo(const std::string& filePath) {
	std::ofstream os(filePath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_physicsSettings),
			CEREAL_NVP(m_alphaTrailSettings),
			CEREAL_NVP(m_colorSettings),
			CEREAL_NVP(m_windSettings),
			CEREAL_NVP(m_particleSystemSettings)
		);
	}
}

void Settings::deserializeFrom(const std::string& filePath) {
	std::ifstream is(filePath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			m_physicsSettings,
			m_alphaTrailSettings,
			m_colorSettings,
			m_windSettings,
			m_particleSystemSettings
		);
	}
}