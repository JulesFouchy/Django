#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include <cereal/archives/json.hpp>
#include <fstream>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Constants/FolderPath.h"

template <typename T>
struct Preset {
	std::string name;
	T values;
};

template <typename T>
class Presets {
public:
	Presets(const char* fileExtension)
		: m_fileExtension(fileExtension + std::string(".")),
		m_savePresetAs(findPlaceholderName(FolderPath::Settings)),
		m_nameAvailable(true)
	{
		loadPresetsFrom(FolderPath::Settings);
		setToPlaceholderSetting();
	}
	~Presets() = default;

	bool ImGui(T* settingValues) {
		ImGui::Separator();
		bool b = false;
		if (ImGui::BeginCombo("Presets", m_currentPresetName.c_str(), 0)) {
			for (size_t i = 0; i < m_presets.size(); i++) {
				if (ImGui::Selectable(m_presets[i].name.c_str(), false)) {
					b = true;
					m_currentPresetName = m_presets[i].name;
					*settingValues = m_presets[i].values;
				}
			}
			ImGui::EndCombo();
		}
		if (m_nameAvailable) {
			if (ImGui::Button("Save settings")) {
				savePresetTo(*settingValues, FolderPath::Settings);
			}
			ImGui::SameLine();
			ImGui::Text("as");
		}
		else {
			ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.f), "Name already used :");
		}
		ImGui::SameLine();
		ImGui::PushID(138571);
		if (ImGui::InputText("", &m_savePresetAs)) {
			m_nameAvailable = !MyFile::Exists(FolderPath::Settings + "/" + m_fileExtension + m_savePresetAs + ".json");
		}
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

	std::string findPlaceholderName(const std::string& folderPath) {
		int i = 1;
		std::string name = "MyPreset";
		while (MyFile::Exists(folderPath + "/" + m_fileExtension + name + ".json")) {
			name = "MyPreset(" + std::to_string(i) + ")";
			i++;
		}
		return name;
	}

	void loadPresetsFrom(const std::string& folderPath) {
		for (const auto& file : fs::directory_iterator(folderPath)) {
			if (!file.path().filename().replace_extension("").replace_extension(".").string().compare(m_fileExtension)) {
				std::string name = file.path().filename().replace_extension("").extension().string().erase(0, 1);
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
		std::ofstream os(folderPath + "/" + m_fileExtension + m_savePresetAs + ".json");
		{
			cereal::JSONOutputArchive archive(os);
			archive(
				settingValues
			);
		}
		// Add it to current list
		m_presets.push_back({ m_savePresetAs, settingValues });
		sort();
		// Give the name to the selected preset
		m_currentPresetName = m_savePresetAs;
		// Find new placeholder name
		m_savePresetAs = findPlaceholderName(folderPath);
	}

private:
	const std::string m_fileExtension;
	std::string m_currentPresetName;
	std::vector<Preset<T>> m_presets;
	std::string m_savePresetAs;
	bool m_nameAvailable;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
	}
};