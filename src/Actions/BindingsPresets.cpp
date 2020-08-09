#include "BindingsPresets.h"

#include "KeyBindings.h"

#include <filesystem>
namespace fs = std::filesystem;
#include <cereal/archives/json.hpp>
#include <fstream>
#include "Helper/String.h"
#include "Helper/File.h"
#include "Constants/SettingsFolder.h"


BindingsPresets::BindingsPresets()
	: m_savePresetAs(findPlaceholderName()),
	m_nameAvailable(true)
{
	loadPresets();
	if (MyFile::Exists(djg::SettingsFolder + "/lastSessionBindingsPreset.json")) {
		std::ifstream is(djg::SettingsFolder + "/lastSessionBindingsPreset.json");
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
}

BindingsPresets::~BindingsPresets() {
	std::ofstream os(djg::SettingsFolder + "/lastSessionBindingsPreset.json");
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
	}
}

std::string BindingsPresets::getFullPath(const std::string& name) {
	return djg::SettingsFolder + "/djgBindings." + name + ".json";
}

void BindingsPresets::ImGui(KeyBindings& keyBindings) {
	if (ImGui::Button("Reset bindings")) {
		keyBindings.resetBindings();
		setPlaceholderPresetName();
	}
	if (ImGui::BeginCombo("Presets", m_currentPresetName.c_str(), 0)) {
		for (const BindingsPreset& preset : m_presets) {
			if (ImGui::Selectable(preset.name.c_str(), false)) {
				m_currentPresetName = preset.name;
				keyBindings.setupBindings(preset.filepath);
			}
		}
		ImGui::EndCombo();
	}
	if (m_nameAvailable) {
		if (ImGui::Button("Save bindings")) {
			savePresetTo(getFullPath(m_savePresetAs), keyBindings);
		}
		ImGui::SameLine();
		ImGui::Text("as");
	}
	else {
		ImGui::TextColored(ImVec4(0.74, 0.04, 0.04, 1), "Name already used :");
	}
	ImGui::SameLine();
	ImGui::PushID(18571);
	if (ImGui::InputText("", &m_savePresetAs)) {
		m_nameAvailable = !MyFile::Exists(getFullPath(m_savePresetAs));
	}
	ImGui::PopID();
}

void BindingsPresets::setPlaceholderPresetName() {
	m_currentPresetName = "Unsaved bindings...";
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
	for (const auto& entry : fs::directory_iterator(djg::SettingsFolder)) {
		if (!MyString::FileName(MyString::FileName(entry.path().string())).compare("djgBindings"))
			m_presets.emplace_back(entry.path().string());
	}
	sort();
}

void BindingsPresets::savePresetTo(const std::string& filepath, KeyBindings& keyBindings) {
	keyBindings.serializeBindings(filepath);
	// Add it to current list
	m_presets.emplace_back(filepath);
	sort();
	// Give the name to the selected preset
	m_currentPresetName = m_savePresetAs;
	// Find new placeholder name
	m_savePresetAs = findPlaceholderName();
}