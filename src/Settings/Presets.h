#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include <cereal/archives/json.hpp>
#include <fstream>
#include "Helper/String.h"

template <typename T>
struct Preset {
	std::string name;
	T values;
};

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
			for (size_t i = 0; i < m_presets.size(); i++) {
				if (ImGui::Selectable(m_presets[i].name.c_str(), false)) {
					m_currentPresetName = m_presets[i].name.c_str();
					*settingValues = m_presets[i].values;
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
		ImGui::PushID(138571);
		ImGui::InputText("", &m_savePresetAs);
		ImGui::PopID();
		return b;
	}

	void setToPlaceholderSetting() {
		m_currentPresetName = "Unsaved settings...";
	}

private:
	void sort() {
		// Case insensitive alphabetical sort
		std::sort(m_presets.begin(), m_presets.end(), [](const Preset<T>& l, const Preset<T>& r) {
			return MyString::ToLower(l.name) < MyString::ToLower(r.name);
		});
		// Put Default first
		for (size_t i = 0; i < m_presets.size(); ++i) {
			if (!m_presets[i].name.compare("Default")) {
				Preset<T> prst = m_presets[i];
				std::copy_backward(m_presets.begin(), i + m_presets.begin(), i + 1 + m_presets.begin());
				m_presets[0] = prst;
				break;
			}
		}
	}

	void loadPresetsFrom(const std::string& folderPath) {
		for (const auto& file : fs::directory_iterator(folderPath)) {
			if (!file.path().filename().replace_extension("").extension().string().compare(m_fileExtension)) {
				std::string name = file.path().filename().replace_extension("").replace_extension("").string();
				T values;
				std::ifstream is(file.path());
				{
					cereal::JSONInputArchive archive(is);
					archive(
						values
					);
				}
				m_presets.push_back({ name, values });
			}
		}
		sort();
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
		m_presets.push_back({ m_savePresetAs, settingValues });
		sort();
	}

private:
	const std::string m_fileExtension;
	std::vector<Preset<T>> m_presets;
	const char* m_currentPresetName;
	std::string m_savePresetAs;
};