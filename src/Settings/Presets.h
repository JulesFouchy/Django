#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include <cereal/archives/json.hpp>
#include <fstream>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Constants/Path.h"
#include "Helper/MyImGui.h"
#include <boxer/boxer.h>

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
		m_savePresetAs(findPlaceholderName(Path::Presets))
	{
		loadPresetsFrom(Path::Presets);
	}
	~Presets() = default;

	inline const std::string& getPresetName() { return m_currentPresetName; }
	inline void setPresetName(const std::string& newName) { m_currentPresetName = newName; }

	bool ImGuiDropdown(const char* label, T* settingValues) {
		bool b = false;
		if (ImGui::BeginCombo(label, m_currentPresetName.c_str(), 0)) {
			for (size_t i = 0; i < m_presets.size(); i++) {
				if (ImGui::Selectable(m_presets[i].name.c_str(), false)) {
					b = true;
					m_currentPresetName = m_presets[i].name;
					m_currentPresetIdx = i;
					*settingValues = m_presets[i].values;
				}
			}
			ImGui::EndCombo();
		}
		return b;
	}

	bool ImGui(T* settingValues) {
		ImGui::Separator();
		bool b = ImGuiDropdown("Presets", settingValues);
		if (LiveMode::IsOff()) {
			// Save preset
			if (!m_currentPresetName.compare("Unsaved settings...")) {
				if (m_nameAvailable) {
					if (!m_nameContainsDots) {
						if (ImGui::Button("Save settings")) {
							savePresetTo(*settingValues, Path::Presets);
						}
						ImGui::SameLine();
						ImGui::Text("as");
					}
					else {
						ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.f), "You can't have dots (.) in the name");
					}
				}
				else {
					ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.f), "Name already used :");
				}
				ImGui::SameLine();
				ImGui::PushID(138571);
				if (ImGui::InputText("", &m_savePresetAs)) {
					m_nameAvailable = !MyFile::Exists(Path::Presets + "/" + m_fileExtension + m_savePresetAs + ".json") && m_savePresetAs.compare("Unsaved settings...");
					m_nameContainsDots = m_savePresetAs.find(".") != std::string::npos;
				}
				ImGui::PopID();
				m_bRenamePopupOpenThisFrame = false;
			}
			// Rename / Delete preset
			else {
				if (MyImGui::BeginPopupContextMenuFromButton("Rename")) {
					if (!m_bRenamePopupOpenLastFrame) {
						m_newPresetName = m_currentPresetName;
					}
					ImGui::PushID(16879654123594);
					ImGui::InputText("", &m_newPresetName);
					ImGui::PopID();
					ImGui::EndPopup();
					m_bRenamePopupOpenThisFrame = true;
				}
				else {
					m_bRenamePopupOpenThisFrame = false;
					if (m_bRenamePopupOpenLastFrame) {
						computeCurrentPresetIdx(); // Could have been changed by playing a record
						if (m_currentPresetIdx != -1) {
							const std::string newPath = Path::Presets + "/" + m_fileExtension + m_newPresetName + ".json";
							if (!MyFile::Exists(newPath)) {
								std::filesystem::rename(
									Path::Presets + "/" + m_fileExtension + m_currentPresetName + ".json",
									newPath
								);
								m_currentPresetName = m_newPresetName;
								m_presets[m_currentPresetIdx].name = m_newPresetName;
								sort();
							}
							else {
								boxer::show(("\"" + m_newPresetName + "\" already exists.").c_str(), "Renaming failed", boxer::Style::Warning);
							}
						}
						else {
							boxer::show("Actually this preset isn't saved on your computer, it was just set by a record.\nYou will now be able to \"Save settings\" if you want to.", "Renaming failed", boxer::Style::Info);
							findPlaceholderName(Path::Presets);
							setToPlaceholderSetting();
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete")) {
					if (boxer::show(("\"" + m_currentPresetName + "\" will be deleted. Are you sure ?").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo) == boxer::Selection::Yes) {
						computeCurrentPresetIdx(); // Could have been changed by playing a record
						if (m_currentPresetIdx != -1) {
							std::filesystem::remove(
								Path::Presets + "/" + m_fileExtension + m_currentPresetName + ".json"
							);
							m_presets.erase(m_currentPresetIdx + m_presets.begin());
						}
						else {
							boxer::show("Actually this preset isn't saved on your computer, it was just set by a record.\nYou will now be able to \"Save settings\" if you want to.", "Deleting failed", boxer::Style::Info);
						}
						findPlaceholderName(Path::Presets);
						setToPlaceholderSetting();
					}
				}
			}
			m_bRenamePopupOpenLastFrame = m_bRenamePopupOpenThisFrame;
			return b;
		}
		else {
			m_bRenamePopupOpenThisFrame = false;
		}
	}

	void setToPlaceholderSetting() {
		m_currentPresetName = "Unsaved settings...";
		m_currentPresetIdx = -1;
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
		//
		computeCurrentPresetIdx();
	}

	void computeCurrentPresetIdx() {
		for (size_t i = 0; i < m_presets.size(); ++i) {
			if (!m_presets[i].name.compare(m_currentPresetName)) {
				m_currentPresetIdx = i;
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
		if (MyFile::Exists(folderPath)) {
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
	}

	void savePresetTo(T& settingValues, const std::string& folderPath) {
		MyFile::CreateFolderIfDoesntExist(folderPath);
		std::ofstream os(folderPath + "/" + m_fileExtension + m_savePresetAs + ".json");
		{
			cereal::JSONOutputArchive archive(os);
			archive(
				settingValues
			);
		}
		// Add it to current list
		m_presets.push_back({ m_savePresetAs, settingValues });
		// Give the name to the selected preset
		m_currentPresetName = m_savePresetAs;
		//
		sort();
		// Find new placeholder name
		m_savePresetAs = findPlaceholderName(folderPath);
	}

private:
	const std::string m_fileExtension;
	std::string m_currentPresetName = "Unsaved settings...";
	size_t m_currentPresetIdx = -1;
	std::vector<Preset<T>> m_presets;
	std::string m_savePresetAs;
	bool m_nameAvailable = true;
	bool m_nameContainsDots = false;
	std::string m_newPresetName;
	bool m_bRenamePopupOpenLastFrame = false;
	bool m_bRenamePopupOpenThisFrame = false;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void save(Archive& archive, std::uint32_t const version) const
	{
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
	}
	template <class Archive>
	void load(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
		computeCurrentPresetIdx();
	}
};
