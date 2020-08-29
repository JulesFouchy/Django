#include "Settings.h"

#include "StateModifier.h"

#include <cereal/archives/json.hpp>
#include <fstream>

void Settings::ImGuiWindows(StateModifier& stateModifier) {
	ImGui::Begin("Trail");
		m_alphaTrailSettings.ImGui(m_colorSettings.backgroundColor(), stateModifier);
	ImGui::End();
	ImGui::Begin("Colors");
		m_colorSettings.ImGui(stateModifier);
	ImGui::End();
	ImGui::Begin("Wind");
		m_windSettings.ImGui(stateModifier);
	ImGui::End();
	ImGui::Begin("Physics");
		m_physicsSettings.ImGui(stateModifier);
	ImGui::End();
	ImGui::Begin("Particles");
		m_particleSystemSettings.ImGui(stateModifier);
	ImGui::End();
}

void Settings::apply(StateModifier& stateModifier) {
	m_windSettings.apply(stateModifier); // must be before particleSystemSettings
	m_particleSystemSettings.apply(stateModifier);
	m_alphaTrailSettings.apply(stateModifier);
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