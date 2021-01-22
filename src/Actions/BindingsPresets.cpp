#include "BindingsPresets.h"

#include "KeyBindings.h"

#include <filesystem>
namespace fs = std::filesystem;
#include <cereal/archives/json.hpp>
#include <fstream>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Helper/MyImGui.h"
#include "Constants/FolderPath.h"
#include <Boxer/boxer.h>

BindingsPresets::BindingsPresets()
	: m_savePresetAs(findPlaceholderName())
{
	const std::string& path = FolderPath::LastSession_BindingsPreset;
	if (MyFile::Exists(path)) {
		std::ifstream is(path);
		{
			cereal::JSONInputArchive archive(is);
			archive(
				CEREAL_NVP(m_currentPresetName)
			);
		}
	}
	else {
		setPlaceholderPresetName();
	}
	loadPresets();
}

BindingsPresets::~BindingsPresets() {
	std::ofstream os(FolderPath::LastSession_BindingsPreset);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
	}
}

std::string BindingsPresets::getFullPath(const std::string& name) {
	return FolderPath::Presets + "/djgBindings." + name + ".json";
}

void BindingsPresets::ImGui(KeyBindings& keyBindings) {
	if (ImGui::Button("Reset bindings")) {
		keyBindings.resetBindings();
		setPlaceholderPresetName();
	}
	if (ImGui::BeginCombo("Presets", m_currentPresetName.c_str(), 0)) {
		for (size_t i = 0; i < m_presets.size(); ++i) {
			if (ImGui::Selectable(m_presets[i].name.c_str(), false)) {
				m_currentPresetName = m_presets[i].name;
				m_currentPresetIdx = i;
				keyBindings.setupBindings(m_presets[i].filepath);
			}
		}
		ImGui::EndCombo();
	}
	// Save / Edit preset
	if (!m_currentPresetName.compare("Unsaved bindings...")) {
		if (m_nameAvailable) {
			if (!m_nameContainsDots) {
				if (ImGui::Button("Save bindings")) {
					savePresetTo(getFullPath(m_savePresetAs), keyBindings);
				}
				ImGui::SameLine();
				ImGui::Text("as");
			}
			else {
				ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.f), "You can't have dots (.) in the name");
			}
		}
		else {
			ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.0f), "Name already used :");
		}
		ImGui::SameLine();
		ImGui::PushID(18571);
		if (ImGui::InputText("", &m_savePresetAs)) {
			m_nameAvailable = !MyFile::Exists(getFullPath(m_savePresetAs));
			m_nameContainsDots = m_savePresetAs.find(".") != std::string::npos;
		}
		ImGui::PopID();
		m_bRenamePopupOpenThisFrame = false;
	}
	else {
		// Rename
		if (MyImGui::BeginPopupContextMenuFromButton("Rename")) {
			if (!m_bRenamePopupOpenLastFrame) {
				m_newPresetName = m_currentPresetName;
			}
			ImGui::PushID(1687965412465424);
			ImGui::InputText("", &m_newPresetName);
			ImGui::PopID();
			ImGui::EndPopup();
			m_bRenamePopupOpenThisFrame = true;
		}
		else {
			m_bRenamePopupOpenThisFrame = false;
			if (m_bRenamePopupOpenLastFrame && m_currentPresetIdx != -1) {
				const std::string newPath = FolderPath::Presets + "/djgBindings." + m_newPresetName + ".json";
				if (!MyFile::Exists(newPath)) {
					std::filesystem::rename(
						FolderPath::Presets + "/djgBindings." + m_currentPresetName + ".json",
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
		}
		ImGui::SameLine();
		// Delete
		if (ImGui::Button("Delete")) {
			if (boxer::show(("\"" + m_currentPresetName + "\" will be deleted. Are you sure ?").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo) == boxer::Selection::Yes) {
				std::filesystem::remove(
					FolderPath::Presets + "/djgBindings." + m_currentPresetName + ".json"
				);
				m_presets.erase(m_currentPresetIdx + m_presets.begin());
				findPlaceholderName();
				setPlaceholderPresetName();
			}
		}
	}
	m_bRenamePopupOpenLastFrame = m_bRenamePopupOpenThisFrame;
}

void BindingsPresets::setPlaceholderPresetName() {
	m_currentPresetName = "Unsaved bindings...";
	m_currentPresetIdx = -1;
}

void BindingsPresets::sort() {
	// Case insensitive alphabetical sort
	std::sort(m_presets.begin(), m_presets.end(), [](const BindingsPreset& l, const BindingsPreset& r) {
		return MyString::ToLower(l.name) < MyString::ToLower(r.name);
	});
	// Put Default first
	for (size_t i = 0; i < m_presets.size(); ++i) {
		if (!m_presets[i].name.compare("Default")) {
			BindingsPreset prst = m_presets[i];
			std::copy_backward(m_presets.begin(), i + m_presets.begin(), i + 1 + m_presets.begin());
			m_presets[0] = prst;
			break;
		}
	}
	// Find m_currentPresetIdx
	for (size_t i = 0; i < m_presets.size(); ++i) {
		if (!m_presets[i].name.compare(m_currentPresetName)) {
			m_currentPresetIdx = i;
			break;
		}
	}
}

std::string BindingsPresets::findPlaceholderName() {
	int i = 1;
	std::string name = "MyBindings";
	while (MyFile::Exists(getFullPath(name))) {
		name = "MyBindings(" + std::to_string(i) + ")";
		i++;
	}
	return name;
}

void BindingsPresets::loadPresets() {
	if (MyFile::Exists(FolderPath::Presets)) {
		for (const auto& entry : fs::directory_iterator(FolderPath::Presets)) {
			if (!MyString::FileName(MyString::FileName(entry.path().string())).compare("djgBindings"))
				m_presets.emplace_back(entry.path().string());
		}
		sort();
	}
}

void BindingsPresets::savePresetTo(const std::string& filepath, KeyBindings& keyBindings) {
	keyBindings.serializeBindings(filepath);
	// Add it to current list
	m_presets.emplace_back(filepath);
	// Give the name to the selected preset
	m_currentPresetName = m_savePresetAs;
	//
	sort();
	// Find new placeholder name
	m_savePresetAs = findPlaceholderName();
}