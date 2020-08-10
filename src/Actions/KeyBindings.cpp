#include "KeyBindings.h"

#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"

#include <imgui/imgui_internal.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/tuple.hpp>
#include <fstream>
#include "Helper/File.h"

#include "Constants/SettingsFolder.h"

static constexpr float KEY_SIZE = 55.0f;
static constexpr float KEY_OFFSET_PROP = 0.2f;
static constexpr float KEY_ROUNDING = 10.0f;

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
	ActionIdentifier actionIdentifier;
	SDL_Scancode scancode;

	ActionIdentifierAndBinding(const std::string& name, ActionType type, SDL_Scancode scancode)
		: actionIdentifier(name, type), scancode(scancode)
	{}
	ActionIdentifierAndBinding() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(actionIdentifier),
			CEREAL_NVP(scancode)
		);
	}
};

KeyBindings::KeyBindings() {
	addAction(textAction, ActionType_MISCELLANEOUS);
	addAction(rerollRandomAction, ActionType_MISCELLANEOUS);

	for (SDL_Scancode key : firstRow)
		allKeys.push_back(key);
	for (SDL_Scancode key : secondRow)
		allKeys.push_back(key);
	for (SDL_Scancode key : thirdRow)
		allKeys.push_back(key);

	int nbKeys;
	SDL_GetKeyboardState(&nbKeys);
	m_keyReleasedLastDate = new Uint32[nbKeys];
	for (int i = 0; i < nbKeys; ++i)
		m_keyReleasedLastDate[i] = 0;
}
KeyBindings::~KeyBindings() {
	serializeBindings(djg::SettingsFolder + "/lastSessionBindings.json");
	for (ActionBinding action : m_allActionsOwner) {
		GLCall(glDeleteTextures(1, &action.action.thumbnailTextureID));
	}
	delete[] m_keyReleasedLastDate;
}

void KeyBindings::onKeyUp(SDL_Scancode scancode) {
	if (!ImGui::GetIO().WantTextInput)
		m_keyReleasedLastDate[scancode] = SDL_GetTicks();
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

void KeyBindings::swapBindings(ActionBinding* actionBinding1, ActionBinding* actionBinding2) {
	//SDL_Scancode scancode1 = actionBinding1->scancode;
	//SDL_Scancode scancode2 = actionBinding2->scancode;
	//setBinding(actionBinding1, scancode2);
	//setBinding(actionBinding2, scancode1);
	//setBinding(actionBinding1, scancode2);
	std::swap(actionBinding1->scancode, actionBinding2->scancode);
	if (actionBinding1->scancode != SDL_SCANCODE_UNKNOWN)
		m_boundActions[actionBinding1->scancode] = actionBinding1;
	if (actionBinding2->scancode != SDL_SCANCODE_UNKNOWN)
		m_boundActions[actionBinding2->scancode] = actionBinding2;
}

void KeyBindings::resetBindings() {
	setupBindings("");
}

void KeyBindings::setupBindings(const std::string& presetFilepath, bool clearExistingBndings) {
	if (clearExistingBndings)
		clearAllBindings();
	std::vector<ActionIdentifier> ignoredActions;
	if (MyFile::Exists(presetFilepath))
		ignoredActions = readBindingsFrom(presetFilepath);
	setupMiscellaneousBindings();
	// Layout
	for (const auto& kv : m_allActionsByType[ActionType::LAYOUT]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), ActionIdentifier(kv.second->action.name, kv.second->action.type)) == ignoredActions.end()) {
			if (!hasBinding(kv.second)) {
				SDL_Scancode scancode = findFirstFromLeft(secondRow);
				if (scancode != SDL_SCANCODE_UNKNOWN)
					setBinding(kv.second, scancode);
				else
					spdlog::warn("Couldn't give a binding to '{}' layout because there was no more room on second row !", kv.second->action.name);
			}
		}
	}
	// Shape
	for (const auto& kv : m_allActionsByType[ActionType::SHAPE]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), ActionIdentifier(kv.second->action.name, kv.second->action.type)) == ignoredActions.end()) {
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
	}
	// SVG Shape
	for (const auto& kv : m_allActionsByType[ActionType::SVG_SHAPE]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), ActionIdentifier(kv.second->action.name, kv.second->action.type)) == ignoredActions.end()) {
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
	}
	// Layout
	for (const auto& kv : m_allActionsByType[ActionType::STANDALONE]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), ActionIdentifier(kv.second->action.name, kv.second->action.type)) == ignoredActions.end()) {
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
}

void KeyBindings::clearAllBindings() {
	for (ActionBinding& actionBinding : m_allActionsOwner)
		setBinding(&actionBinding, SDL_SCANCODE_UNKNOWN);
}

void KeyBindings::setupMiscellaneousBindings() {
	setBinding(m_allActionsByType[ActionType_MISCELLANEOUS][textAction.name],         SDL_SCANCODE_RETURN);
	setBinding(m_allActionsByType[ActionType_MISCELLANEOUS][rerollRandomAction.name], SDL_SCANCODE_SPACE);
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

void KeyBindings::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	// Keyboard
	ImGui_KeyboardRow(configManager, partSystem, firstRow,  0.0f);
	ImGui_KeyboardRow(configManager, partSystem, secondRow, KEY_OFFSET_PROP * KEY_SIZE);
	ImGui_KeyboardRow(configManager, partSystem, thirdRow,  KEY_OFFSET_PROP * KEY_SIZE * 2.0f);
	ImGui::NewLine();
	// List of configurations without a binding
	bool b = false;
	bool bSameLine = false;
	for (auto it = m_allActionsByType.begin(); it != m_allActionsByType.end(); ++it) {
		if (!hasBinding(it->second)) {
			bSameLine = false;
			if (!b) {
				ImGui::Separator();
				ImGui::Text("Configurations without bindings : ");
				ImGui::SameLine();
				if (ImGui::Button("Auto-distribute them")) {
					setupBindings("", false);
				}
				b = true;
			}
			ActionBinding* actionBinding = it->second;
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			if (actionBinding->action.thumbnailTextureID != -1) {
				ImGui_DragNDropKey(SDL_SCANCODE_UNKNOWN, actionBinding);
			}
			else {
				ImGui::TextDisabled(actionBinding->action.name.c_str());
			}
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + KEY_SIZE;
			if (next_button_x2 < window_visible_x2) {
				ImGui::SameLine();
				bSameLine = true;
			}
		}
	}
	if (bSameLine)
		ImGui::NewLine(); // cancel the last SameLine that wasn't used
	// Presets
	ImGui::Separator();
	m_presets.ImGui(*this);
	//
	m_mouseWasDraggingLastFrame = ImGui::IsMouseDragging(0);
}

void KeyBindings::ImGui_KeyboardRow(ConfigManager& configManager, ParticlesSystem& partSystem, const std::vector<SDL_Scancode>& row, float indent) {
	ImGui::Indent(indent);
	for (SDL_Scancode scancode : row) {
		ImGui::PushID((int)scancode + 333);
		ImGui_DragNDropKey(scancode);
		if (ImGui::IsItemDeactivated() && !m_mouseWasDraggingLastFrame) {
			configManager.onKeyPressed(scancode, 0, partSystem); // pass invalid keysym (a.k.a neither a letter nor space) so that it doesn't write if text config is on
			onKeyUp(scancode);
		}
		ImGui::PopID();
		ImGui::SameLine();
	}
	ImGui::NewLine();
	ImGui::Spacing();
}

void KeyBindings::ImGui_DragNDropKey(SDL_Scancode scancode, ActionBinding* actionBinding) {
	// Get key state
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (!actionBinding) {
		auto it = m_boundActions.find(scancode);
		if (it != m_boundActions.end())
			actionBinding = it->second;
	}
	unsigned int textureID = actionBinding ? actionBinding->action.thumbnailTextureID : -1;
	bool isKeyPressed = keyboardState[scancode] && !ImGui::GetIO().WantTextInput;
	// Draw Key
	ImGui_KeyboardKey(scancode, textureID, (bool)actionBinding, isKeyPressed);
	if (actionBinding) {
		// Name on hover for Layout actions and actions with no thumbnail
		if (ImGui::IsItemHovered() && (actionBinding->action.type == ActionType::LAYOUT || actionBinding->action.type == ActionType::STANDALONE || actionBinding->action.thumbnailTextureID == -1)) {
			ImGui::BeginTooltip();
			ImGui::Text(actionBinding->action.name.c_str());
			ImGui::EndTooltip();
		}
		// Right-click to delete
		if (scancode != SDL_SCANCODE_UNKNOWN) {
			if (ImGui::BeginPopupContextItem("")) {
				if (ImGui::Button("Remove binding")) {
					setBinding(actionBinding, SDL_SCANCODE_UNKNOWN);
					m_presets.setPlaceholderPresetName();
				}
				ImGui::EndPopup();
			}
		}
	}
	// Drag'n Drop !
	if (actionBinding) {
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("KEYBOARD_KEY", &actionBinding, sizeof(ActionBinding*));
			ImGui_KeyboardKey(SDL_SCANCODE_UNKNOWN, textureID, (bool)actionBinding, false); // Preview
			ImGui::EndDragDropSource();
		}
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("KEYBOARD_KEY"))
		{
			IM_ASSERT(payload->DataSize == sizeof(ActionBinding*));
			if (actionBinding)
				swapBindings(*((ActionBinding**)payload->Data), actionBinding);
			else
				setBinding(*((ActionBinding**)payload->Data), scancode);
			m_presets.setPlaceholderPresetName();
		}
		ImGui::EndDragDropTarget();
	}
}

bool KeyBindings::ImGui_KeyboardKey(SDL_Scancode scancode, unsigned int textureID, bool hasAnActionBound, bool isKeyPressed) {
	//
	ImGuiStyle& style = ImGui::GetStyle();
	//
	ImVec2 p = ImGui::GetCursorScreenPos();
	// Detect clic
	ImGui::PushID((int)scancode+1294);
	ImGui::InvisibleButton("", ImVec2(KEY_SIZE, KEY_SIZE));
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();

	if (is_active) {
		// onKeyUp(scancode);
	}
	// Color fade when key released
	ImU32 col32;
	ImVec4 backgroundColor = style.Colors[hasAnActionBound ? (textureID != -1 ? ImGuiCol_WindowBg : ImGuiCol_Border) : ImGuiCol_FrameBg];
	static constexpr float ttl = 0.3f; // in seconds
	float t = (SDL_GetTicks() - m_keyReleasedLastDate[scancode]) * 0.001f / ttl;
	bool isKeyActiveRecently = isKeyPressed || t < 1.0f;
	if (isKeyActiveRecently) {
		ImVec4 activeColor = style.Colors[ImGuiCol_FrameBgActive];
		ImVec4 frameActiveColor = isKeyPressed 
			? activeColor
			: ImVec4(
				activeColor.x * (1 - t) + backgroundColor.x * t,
				activeColor.y * (1 - t) + backgroundColor.y * t,
				activeColor.z * (1 - t) + backgroundColor.z * t,
				activeColor.w * (1 - t) + backgroundColor.w * t
			);
		frameActiveColor.w *= style.Alpha;
		col32 = ImGui::ColorConvertFloat4ToU32(frameActiveColor);
	}
	else {
		backgroundColor.w *= style.Alpha;
		col32 = is_hovered ? ImGui::GetColorU32(ImGuiCol_FrameBgHovered) : ImGui::ColorConvertFloat4ToU32(backgroundColor);
	}
	//
	ImU32 col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
	ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float fontSize = 1.5f * draw_list->_Data->FontSize;
	ImVec2 pMax = ImVec2(p.x + KEY_SIZE, p.y + KEY_SIZE);
	if (textureID != -1) {
		if (isKeyActiveRecently)
			draw_list->AddRectFilled(p, pMax, col32, KEY_ROUNDING);
		draw_list->AddImageRounded((ImTextureID)textureID, p, pMax, ImVec2(0, 1), ImVec2(1, 0), ImGui::GetColorU32(is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_Border), KEY_ROUNDING);
	}
	else {
		draw_list->AddRectFilled(p, pMax, col32, KEY_ROUNDING);
	}
	draw_list->AddText(NULL, fontSize, ImVec2(p.x + KEY_SIZE * 0.5f - fontSize * 0.25f, p.y + KEY_SIZE * 0.5f - fontSize * 0.45f), col32text, SDL_GetKeyName(SDL_GetKeyFromScancode(scancode)));

	ImGui::PopID();
	return is_active;
}

void KeyBindings::serializeBindings(const std::string& filepath) {
	std::vector<ActionIdentifierAndBinding> boundActionsIdentifiers;
	std::vector<ActionIdentifier> actionsWithNoBinding;
	for (const ActionBinding& actionBinding : m_allActionsOwner) {
		if (actionBinding.scancode != SDL_SCANCODE_UNKNOWN)
			boundActionsIdentifiers.emplace_back(actionBinding.action.name, actionBinding.action.type, actionBinding.scancode);
		else
			actionsWithNoBinding.emplace_back(actionBinding.action.name, actionBinding.action.type);
	}
	std::ofstream os(filepath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			boundActionsIdentifiers,
			actionsWithNoBinding
		);
	}
}

std::vector<ActionIdentifier> KeyBindings::readBindingsFrom(const std::string& filepath) {
	std::vector<ActionIdentifierAndBinding> boundActionsIdentifiers;
	std::vector<ActionIdentifier> actionsWithNoBinding;
	std::ifstream is(filepath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			boundActionsIdentifiers,
			actionsWithNoBinding
		);
	}
	for (const auto& actionIdentifierAndBinding : boundActionsIdentifiers) {
		if (ActionBinding* actionBinding = m_allActionsByType.tryFind(actionIdentifierAndBinding.actionIdentifier.name, actionIdentifierAndBinding.actionIdentifier.type)) {
			setBinding(actionBinding, actionIdentifierAndBinding.scancode);
		}
	}
	return actionsWithNoBinding;
}