#include "KeyBindings.h"

#include <imgui/imgui_internal.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/tuple.hpp>
#include <fstream>
#include "Helper/File.h"

#include "Constants/SettingsFolder.h"

static constexpr float keySize = 55.0f;
static constexpr float keyOffsetProp = 0.2f;

static const Action textAction(
	"Text",
	-1,
	ActionType::TEXT
);
static const Action rerollRandomAction(
	"Reroll random",
	-1,
	ActionType::REROLL_RANDOM
);

struct ActionIdentifierAndBinding {
	std::string name;
	ActionType type;
	SDL_Scancode scancode;

	ActionIdentifierAndBinding(const std::string& name, ActionType type, SDL_Scancode scancode)
		: name(name), type(type), scancode(scancode)
	{}
	ActionIdentifierAndBinding() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(type),
			CEREAL_NVP(scancode)
		);
	}
};

KeyBindings::KeyBindings() {
	addAction(textAction, ActionType_MISCELLANEOUS);
	setBinding(&m_allActionsOwner.back(), SDL_SCANCODE_RETURN);
	addAction(rerollRandomAction, ActionType_MISCELLANEOUS);
	setBinding(&m_allActionsOwner.back(), SDL_SCANCODE_SPACE);

	for (SDL_Scancode key : firstRow)
		allKeys.push_back(key);
	for (SDL_Scancode key : secondRow)
		allKeys.push_back(key);
	for (SDL_Scancode key : thirdRow)
		allKeys.push_back(key);
}

KeyBindings::~KeyBindings() {
	serializeBindings(djg::SettingsFolder + "/lastSessionBindings.json");
	for (ActionBinding action : m_allActionsOwner) {
		GLCall(glDeleteTextures(1, &action.action.thumbnailTextureID));
	}
}

const Action* KeyBindings::getAction(SDL_Scancode scancode) {
	auto it = m_boundActions.find(scancode);
	if (it != m_boundActions.end()) {
		return &it->second->action;
	}
	return nullptr;
}

void KeyBindings::addAction(Action action) {
	addAction(action, (int)action.type);
}

void KeyBindings::addAction(Action action, int type) {
	m_allActionsOwner.emplace_back(action);
	//
	auto& map = m_allActionsByType[type];
	if (map.find(action.name) != map.end())
		spdlog::warn("There is already an action called '{}' and with the same type ({}) !\nNot adding this one !", action.name, type);
	else {
		map[action.name] = &m_allActionsOwner.back();
	}
}

void KeyBindings::setBinding(ActionBinding* actionBinding, SDL_Scancode scancode) {
	// Erase the binding previously attached to scancode
	if (m_boundActions.find((int)scancode) != m_boundActions.end()) {
		m_boundActions[scancode]->scancode = SDL_SCANCODE_UNKNOWN;
	}
	// Erase the old binding of actionBinding
	if (m_boundActions.find((int)actionBinding->scancode) != m_boundActions.end()) {
		m_boundActions.erase(actionBinding->scancode);
	}
	// Set the new binding
	actionBinding->scancode = scancode;
	m_boundActions[scancode] = actionBinding;
}

void KeyBindings::setupBindings() {
	if (MyFile::Exists(djg::SettingsFolder + "/lastSessionBindings.json")) {
		clearAllBindings();
		readBindingsFrom(djg::SettingsFolder + "/lastSessionBindings.json");
	}
	// Layout
	for (const auto& kv : m_allActionsByType[ActionType::LAYOUT]) {
		if (!hasBinding(kv.second)) {
			SDL_Scancode scancode = findFirstFromLeft(secondRow);
			if (scancode != SDL_SCANCODE_UNKNOWN)
				setBinding(kv.second, scancode);
			else
				spdlog::warn("Couldn't give a binding to '{}' layout because there was no more room on second row !", kv.second->action.name);
		}
	}
	// Shape
	for (const auto& kv : m_allActionsByType[ActionType::SHAPE]) {
		if (!hasBinding(kv.second)) {
			SDL_Scancode scancode = findFirstFromLeft(firstRow);
			if (scancode == SDL_SCANCODE_UNKNOWN)
				scancode = findFirstFromRight(secondRow);
			if (scancode != SDL_SCANCODE_UNKNOWN)
				setBinding(kv.second, scancode);
			else
				spdlog::warn("Couldn't give a binding to '{}' shape because there was no more room on first and second row !", kv.second->action.name);
		}
	}
	// SVG Shape
	for (const auto& kv : m_allActionsByType[ActionType::SVG_SHAPE]) {
		if (!hasBinding(kv.second)) {
			SDL_Scancode scancode = findFirstFromLeft(firstRow);
			if (scancode == SDL_SCANCODE_UNKNOWN)
				scancode = findFirstFromRight(secondRow);
			if (scancode != SDL_SCANCODE_UNKNOWN)
				setBinding(kv.second, scancode);
			else
				spdlog::warn("Couldn't give a binding to '{}' svg-shape because there was no more room on first and second row !", kv.second->action.name);
		}
	}
	// Layout
	for (const auto& kv : m_allActionsByType[ActionType::STANDALONE]) {
		if (!hasBinding(kv.second)) {
			SDL_Scancode scancode = findFirstFromLeft(thirdRow);
			if (scancode == SDL_SCANCODE_UNKNOWN) {
				scancode = findFirstFromRight(secondRow);
			}
			if (scancode != SDL_SCANCODE_UNKNOWN)
				setBinding(kv.second, scancode);
			else
				spdlog::warn("Couldn't give a binding to '{}' standalone because there was no more room on third and second row !", kv.second->action.name);
		}
	}
}

void KeyBindings::clearAllBindings() {
	for (ActionBinding& actionBinding : m_allActionsOwner)
		setBinding(&actionBinding, SDL_SCANCODE_UNKNOWN);
}

bool KeyBindings::hasBinding(const ActionBinding* actionBinding) {
	return actionBinding->scancode != SDL_SCANCODE_UNKNOWN;
}

bool KeyBindings::isKeyAvailable(SDL_Scancode scancode) {
	return m_boundActions.find(scancode) == m_boundActions.end();
}

SDL_Scancode KeyBindings::findFirstFromLeft(std::vector<SDL_Scancode> row) {
	for (size_t i = 0; i < row.size(); ++i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return SDL_SCANCODE_UNKNOWN;
}

SDL_Scancode KeyBindings::findFirstFromRight(std::vector<SDL_Scancode> row) {
	for (int i = row.size() - 1; i >= 0; --i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return SDL_SCANCODE_UNKNOWN;
}

void KeyBindings::ImGui() {
	ImGui_KeyboardRow(firstRow,  0.0f);
	ImGui_KeyboardRow(secondRow, keyOffsetProp * keySize);
	ImGui_KeyboardRow(thirdRow,  keyOffsetProp * keySize * 2.0f);
	bool b = false;
	for (auto it = m_allActionsByType.begin(); it != m_allActionsByType.end(); ++it) {
		if (!hasBinding(it->second)) {
			if (!b) {
				ImGui::Text("Configurations without bindings : ");
				b = true;
			}
			ImGui::TextDisabled(it->second->action.name.c_str());
		}
	}
}

void KeyBindings::ImGui_KeyboardRow(const std::vector<SDL_Scancode>& row, float indent) {
	// Get keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	//
	ImGui::Indent(indent);
	for (SDL_Scancode scancode : row) {
		ImGui::PushID((int)scancode + 333);
		// Key
		auto it = m_boundActions.find(scancode);
		bool bHasAnActionBound = it != m_boundActions.end();
		unsigned int textureID = bHasAnActionBound ? it->second->action.thumbnailTextureID : -1;
		if (ImGui_KeyboardKey(scancode, textureID, bHasAnActionBound, keyboardState[scancode]))
			ImGui::OpenPopup("Config list");
		// Config list
		if (ImGui::BeginPopup("Config list")) {
			for (auto it = m_allActionsByType.begin(); it != m_allActionsByType.end(); ++it) {
				SDL_Scancode cfgScancode = (SDL_Scancode)it->second->scancode;
				const Action& action = it->second->action;
				if (ImGui::Selectable(action.name.c_str(), scancode == cfgScancode)) {
					setBinding(it->second, scancode);
				}
			}
			ImGui::EndPopup();
		}
		//
		ImGui::PopID();
		ImGui::SameLine();
	}
	ImGui::NewLine();
	ImGui::Spacing();
}

bool KeyBindings::ImGui_KeyboardKey(SDL_Scancode scancode, unsigned int textureID, bool hasAnActionBound, bool isKeyPressed) {
	//
	ImGuiStyle& style = ImGui::GetStyle();
	//
	ImVec2 p = ImGui::GetCursorScreenPos();
	// Detect clic
	ImGui::PushID((int)scancode+1294);
	ImGui::InvisibleButton("", ImVec2(keySize, keySize));
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();

	if (is_active) {
		//ImVec2 mp = ImGui::GetIO().MousePos;
		//*value_p = atan2f(center.y - mp.y, mp.x - center.x);
	}

	ImU32 col32 = ImGui::GetColorU32(is_active || isKeyPressed ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : hasAnActionBound ? ImGuiCol_Border : ImGuiCol_FrameBg);
	ImU32 col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
	ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float fontSize = 1.5f * draw_list->_Data->FontSize;
	ImVec2 pMax = ImVec2(p.x + keySize, p.y + keySize);
	float rounding = 10.0f;
	if (textureID != -1) {
		if (is_active || isKeyPressed)
			draw_list->AddRectFilled(p, pMax, ImGui::GetColorU32(ImGuiCol_FrameBgActive), rounding);
		draw_list->AddImageRounded((ImTextureID)textureID, p, pMax, ImVec2(0, 1), ImVec2(1, 0), col32, rounding);
	}
	else {
		draw_list->AddRectFilled(p, pMax, col32, rounding);
	}
	draw_list->AddText(NULL, fontSize, ImVec2(p.x + keySize * 0.5f - fontSize * 0.25f, p.y + keySize * 0.5f - fontSize * 0.45f), col32text, SDL_GetKeyName(SDL_GetKeyFromScancode(scancode)));

	ImGui::PopID();
	return is_active;
}

void KeyBindings::serializeBindings(const std::string& filepath) {
	std::vector<ActionIdentifierAndBinding> boundActionsIdentifiers;
	for (auto& kv : m_boundActions) {
		boundActionsIdentifiers.emplace_back(kv.second->action.name, kv.second->action.type, (SDL_Scancode)kv.first);
	}
	std::ofstream os(filepath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			boundActionsIdentifiers
		);
	}
}

void KeyBindings::readBindingsFrom(const std::string& filepath) {
	std::vector<ActionIdentifierAndBinding> boundActionsIdentifiers;
	std::ifstream is(filepath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			boundActionsIdentifiers
		);
	}
	for (const auto& actionIdentifierAndBinding : boundActionsIdentifiers) {
		if (ActionBinding* actionBinding = m_allActionsByType.tryFind(actionIdentifierAndBinding.name, actionIdentifierAndBinding.type)) {
			setBinding(actionBinding, actionIdentifierAndBinding.scancode);
		}
	}
}