#include "Settings.h"

#include <cereal/archives/json.hpp>
#include <fstream>

void Settings::ImGuiWindows(ShaderPipeline& physicsCompute, ParticlesSystem& partSystem, Configuration& currentConfiguration) {
	ImGui::Begin("Style");
		m_visualSettings.ImGui();
	ImGui::End();
	ImGui::Begin("Wind");
		m_windSettings.ImGui(physicsCompute);
	ImGui::End();
	ImGui::Begin("Physics");
		m_physicsSettings.ImGui(physicsCompute);
	ImGui::End();
	ImGui::Begin("Particles");
		m_particleSystemSettings.ImGui(partSystem, currentConfiguration);
	ImGui::End();
}

void Settings::apply(ShaderPipeline& physicsCompute, ParticlesSystem& partSystem, Configuration& currentConfiguration) {
	m_visualSettings.apply();
	m_windSettings.apply(physicsCompute);
	m_physicsSettings.apply(physicsCompute);
	m_particleSystemSettings.apply(partSystem, currentConfiguration);
}

void Settings::serializeTo(const std::string& filePath) {
	std::ofstream os(filePath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_physicsSettings),
			CEREAL_NVP(m_visualSettings),
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
			m_visualSettings,
			m_windSettings,
			m_particleSystemSettings
		);
	}
}