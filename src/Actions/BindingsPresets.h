#pragma once

#include "Helper/String.h"

class KeyBindings;

struct BindingsPreset {
	std::string filepath;
	std::string name;

	BindingsPreset(const std::string& filepath)
		: filepath(filepath), name(MyString::FileExtension(MyString::FileName(filepath)).erase(0, 1))
	{}
};

class BindingsPresets {
public:
	BindingsPresets();
	~BindingsPresets();

	void ImGui(KeyBindings& keyBindings);
	void setPlaceholderPresetName();

private:
	void sort();
	std::string getFullPath(const std::string& name);
	std::string findPlaceholderName();
	void loadPresets();
	void savePresetTo(const std::string& filepath, KeyBindings& keyBindings);

private:
	std::string m_currentPresetName;
	size_t m_currentPresetIdx = -1;
	std::vector<BindingsPreset> m_presets;
	std::string m_savePresetAs;
	bool m_nameAvailable = true;
	bool m_nameContainsDots = false;
	std::string m_newPresetName;
	bool m_bRenamePopupOpenLastFrame = false;
	bool m_bRenamePopupOpenThisFrame = false;
};