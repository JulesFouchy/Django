#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include <cereal/archives/json.hpp>
#include <fstream>

template <typename T>
class Presets {
public:
	Presets(const char* fileExtension, const std::string& settingsFolderPath)
		: m_fileExtension(std::string(".") + fileExtension)
	{
		loadPresetsFrom(settingsFolderPath);
		setToPlaceholderSetting();
	}
	~Presets() = default;

	bool ImGui(T* settingValues) {
		bool b;
		if (b = ImGui::BeginCombo("Presets", m_currentPresetName, 0)) {
			for (size_t i = 0; i < m_presetsNames.size(); i++) {
				if (ImGui::Selectable(m_presetsNames[i].c_str(), false)) {
					m_currentPresetName = m_presetsNames[i].c_str();
					*settingValues = m_presets[i];
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();
		if (ImGui::Button("Save settings")) {
			savePresetTo(*settingValues, "C:/Dev/Django/settings");
		}
		ImGui::SameLine();
		ImGui::Text("as");
		ImGui::SameLine();
		ImGui::InputText("", &m_savePresetAs);
		return b;
	}

	void setToPlaceholderSetting() {
		m_currentPresetName = "Unsaved settings...";
	}

private:
	void loadPresetsFrom(const std::string& folderPath) {
		for (const auto& file : fs::directory_iterator(folderPath)) {
			if (!file.path().filename().replace_extension("").extension().string().compare(m_fileExtension)) {
				spdlog::warn(file.path().string());
				m_presetsNames.push_back(file.path().filename().replace_extension("").replace_extension("").string());
				T preset;
				std::ifstream is(file.path());
				{
					cereal::JSONInputArchive archive(is);
					archive(
						preset
					);
				}
				m_presets.push_back(preset);
				// Default is always put first in the list
				if (!m_presetsNames.back().compare("Default")) {
					std::swap(m_presetsNames.back(), m_presetsNames[0]);
					std::swap(m_presets.back(), m_presets[0]);
				}
			}
		}
	}

	void savePresetTo(T& settingValues, const std::string& folderPath) {
		std::ofstream os(folderPath + "/" + m_savePresetAs + m_fileExtension + ".json");
		{
			cereal::JSONOutputArchive archive(os);
			archive(
				settingValues
			);
		}
		// Add it to current list
		m_presets.push_back(settingValues);
		m_presetsNames.push_back(m_savePresetAs);
	}

private:
	const std::string m_fileExtension;
	std::vector<T> m_presets;
	std::vector<std::string> m_presetsNames;
	const char* m_currentPresetName;
	std::string m_savePresetAs;
};