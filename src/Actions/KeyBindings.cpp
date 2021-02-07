#include "KeyBindings.h"

#include "StateModifier.h"
#include "Configurations/ConfigManager.h"

#include <imgui/imgui_internal.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/tuple.hpp>
#include <fstream>
#include "Helper/File.h"

#include "Constants/Path.h"

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

struct ActionRefAndBinding {
	ActionRef actionRef;
	GLFW_Keycode keycode;

	ActionRefAndBinding(const ActionRef& actionRef, GLFW_Keycode keycode)
		: actionRef(actionRef), keycode(keycode)
	{}
	ActionRefAndBinding(const std::string& name, ActionType type, GLFW_Keycode keycode)
		: actionRef(name, type), keycode(keycode)
	{}
	ActionRefAndBinding() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(actionRef),
			CEREAL_NVP(keycode)
		);
	}
};

KeyBindings::KeyBindings() {
	addAction(textAction, ActionType_MISCELLANEOUS);
	addAction(rerollRandomAction, ActionType_MISCELLANEOUS);

	int nbKeys;
	SDL_GetKeyboardState(&nbKeys);
	m_keyReleasedLastDate = new Uint32[nbKeys];
	for (int i = 0; i < nbKeys; ++i)
		m_keyReleasedLastDate[i] = 0;

	Cool::Serialization::FromJSON(m_keyboardLayout, Path::LastSession_KeyboardLayout.c_str());
}

KeyBindings::~KeyBindings() {
	serializeBindings(Path::LastSession_Bindings);
	Cool::Serialization::ToJSON(m_keyboardLayout, Path::LastSession_KeyboardLayout.c_str());
	for (ActionBinding action : m_allActionsOwner) {
		GLCall(glDeleteTextures(1, &action.action.thumbnailTextureID));
	}
	delete[] m_keyReleasedLastDate;
}

void KeyBindings::onKeyUp(GLFW_Keycode keycode) {
	if (!ImGui::GetIO().WantTextInput) {
		m_keyboardLayout.onKeyUp(keycode);
		m_keyReleasedLastDate[keycode] = SDL_GetTicks();
	}
}

const Action* KeyBindings::getAction(GLFW_Keycode keycode) {
	auto it = m_boundActions.find(keycode);
	if (it != m_boundActions.end()) {
		return &it->second->action;
	}
	return nullptr;
}

void KeyBindings::addAction(Action action) {
	addAction(action, (int)action.ref.type);
}

void KeyBindings::addAction(Action action, int type) {
	m_allActionsOwner.emplace_back(action);
	//
	auto& map = m_allActionsByType[type];
	if (map.find(action.ref.name) != map.end())
		spdlog::warn("There is already an action called '{}' and with the same type ({}) !\nNot adding this one !", action.ref.name, type);
	else {
		map[action.ref.name] = &m_allActionsOwner.back();
	}
}

void KeyBindings::setBinding(ActionBinding* actionBinding, GLFW_Keycode keycode) {
	// Erase the binding previously attached to scancode
	if (m_boundActions.find(keycode) != m_boundActions.end()) {
		m_boundActions[keycode]->keycode = GLFW_KEY_UNKNOWN;
	}
	// Erase the old binding of actionBinding
	if (m_boundActions.find(actionBinding->keycode) != m_boundActions.end()) {
		m_boundActions.erase(actionBinding->keycode);
	}
	// Set the new binding
	actionBinding->keycode = keycode;
	m_boundActions[keycode] = actionBinding;
}

void KeyBindings::swapBindings(ActionBinding* actionBinding1, ActionBinding* actionBinding2) {
	std::swap(actionBinding1->keycode, actionBinding2->keycode);
	if (actionBinding1->keycode != GLFW_KEY_UNKNOWN)
		m_boundActions[actionBinding1->keycode] = actionBinding1;
	if (actionBinding2->keycode != GLFW_KEY_UNKNOWN)
		m_boundActions[actionBinding2->keycode] = actionBinding2;
}

void KeyBindings::resetBindings() {
	setupBindings("");
}

void KeyBindings::setupBindings(const std::string& presetFilepath, bool clearExistingBndings) {
	if (clearExistingBndings)
		clearAllBindings();
	std::vector<ActionRef> ignoredActions;
	if (MyFile::Exists(presetFilepath))
		ignoredActions = readBindingsFrom(presetFilepath);
	setupMiscellaneousBindings();
	// Layout
	for (const auto& kv : m_allActionsByType[ActionType::LAYOUT]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), kv.second->action.ref) == ignoredActions.end()) {
			if (!hasBinding(kv.second)) {
				GLFW_Keycode keycode = findFirstFromLeft(m_keyboardLayout.secondRow());
				if (keycode != GLFW_KEY_UNKNOWN)
					setBinding(kv.second, keycode);
				else
					spdlog::warn("Couldn't give a binding to '{}' layout because there was no more room on second row !", kv.second->action.ref.name);
			}
		}
	}
	// Shape
	for (const auto& kv : m_allActionsByType[ActionType::SHAPE]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), kv.second->action.ref) == ignoredActions.end()) {
			if (!hasBinding(kv.second)) {
				GLFW_Keycode keycode = findFirstFromLeft(m_keyboardLayout.firstRow());
				if (keycode == GLFW_KEY_UNKNOWN)
					keycode = findFirstFromRight(m_keyboardLayout.secondRow());
				if (keycode != GLFW_KEY_UNKNOWN)
					setBinding(kv.second, keycode);
				else
					spdlog::warn("Couldn't give a binding to '{}' shape because there was no more room on first and second row !", kv.second->action.ref.name);
			}
		}
	}
	// SVG Shape
	for (const auto& kv : m_allActionsByType[ActionType::SVG_SHAPE]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), kv.second->action.ref) == ignoredActions.end()) {
			if (!hasBinding(kv.second)) {
				GLFW_Keycode keycode = findFirstFromLeft(m_keyboardLayout.firstRow());
				if (keycode == GLFW_KEY_UNKNOWN)
					keycode = findFirstFromRight(m_keyboardLayout.secondRow());
				if (keycode != GLFW_KEY_UNKNOWN)
					setBinding(kv.second, keycode);
				else
					spdlog::warn("Couldn't give a binding to '{}' svg-shape because there was no more room on first and second row !", kv.second->action.ref.name);
			}
		}
	}
	// Layout
	for (const auto& kv : m_allActionsByType[ActionType::STANDALONE]) {
		if (std::find(ignoredActions.begin(), ignoredActions.end(), kv.second->action.ref) == ignoredActions.end()) {
			if (!hasBinding(kv.second)) {
				GLFW_Keycode keycode = findFirstFromLeft(m_keyboardLayout.thirdRow());
				if (keycode == GLFW_KEY_UNKNOWN) {
					keycode = findFirstFromRight(m_keyboardLayout.secondRow());
				}
				if (keycode != GLFW_KEY_UNKNOWN)
					setBinding(kv.second, keycode);
				else
					spdlog::warn("Couldn't give a binding to '{}' standalone because there was no more room on third and second row !", kv.second->action.ref.name);
			}
		}
	}
}

void KeyBindings::clearAllBindings() {
	for (ActionBinding& actionBinding : m_allActionsOwner)
		setBinding(&actionBinding, SDL_SCANCODE_UNKNOWN);
}

void KeyBindings::setupMiscellaneousBindings() {
	setBinding(m_allActionsByType[ActionType_MISCELLANEOUS][textAction.ref.name],         SDL_SCANCODE_RETURN);
	setBinding(m_allActionsByType[ActionType_MISCELLANEOUS][rerollRandomAction.ref.name], SDL_SCANCODE_SPACE);
}

bool KeyBindings::hasBinding(const ActionBinding* actionBinding) {
	return actionBinding->keycode != GLFW_KEY_UNKNOWN;
}

bool KeyBindings::isKeyAvailable(GLFW_Keycode keycode) {
	return m_boundActions.find(keycode) == m_boundActions.end();
}

GLFW_Keycode KeyBindings::findFirstFromLeft(const std::vector<GLFW_Keycode>& row) {
	for (size_t i = 0; i < row.size(); ++i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return GLFW_KEY_UNKNOWN;
}

GLFW_Keycode KeyBindings::findFirstFromRight(const std::vector<GLFW_Keycode>& row) {
	for (int i = row.size() - 1; i >= 0; --i) {
		if (isKeyAvailable(row[i]))
			return row[i];
	}
	return GLFW_KEY_UNKNOWN;
}

void KeyBindings::ImGui(StateModifier& stateModifier) {
	// Keyboard
	ImGui_KeyboardRow(m_keyboardLayout.numbersRow(), 0.0f                             , stateModifier);
	ImGui_KeyboardRow(m_keyboardLayout.firstRow(),   0.0f                             , stateModifier);
	ImGui_KeyboardRow(m_keyboardLayout.secondRow(),  KEY_OFFSET_PROP * KEY_SIZE       , stateModifier);
	ImGui_KeyboardRow(m_keyboardLayout.thirdRow(),   KEY_OFFSET_PROP * KEY_SIZE * 2.0f, stateModifier);
	ImGui::NewLine();
	if (LiveMode::IsOff()) {
		m_keyboardLayout.ImGui();
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
					ImGui::TextDisabled(actionBinding->action.ref.name.c_str());
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
	}
	m_mouseWasDraggingLastFrame = ImGui::IsMouseDragging(0);
}

void KeyBindings::ImGui_KeyboardRow(const std::vector<GLFW_Keycode>& row, float indent, StateModifier& stateModifier) {
	ImGui::Indent(indent);
	for (GLFW_Keycode keycode : row) {
		ImGui::PushID(keycode + 333);
		ImGui_DragNDropKey(keycode);
		if (ImGui::IsItemDeactivated() && !m_mouseWasDraggingLastFrame) {
			stateModifier.configManager().onKeyPressed(keycode, 0, stateModifier); // pass invalid keysym (a.k.a neither a letter nor space) so that it doesn't write if text config is on
			onKeyUp(keycode);
		}
		ImGui::PopID();
		ImGui::SameLine();
	}
	ImGui::NewLine();
	ImGui::Spacing();
}

void KeyBindings::ImGui_DragNDropKey(GLFW_Keycode keycode, ActionBinding* actionBinding) {
	// Get key state
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (!actionBinding) {
		auto it = m_boundActions.find(keycode);
		if (it != m_boundActions.end())
			actionBinding = it->second;
	}
	unsigned int textureID = actionBinding ? actionBinding->action.thumbnailTextureID : -1;
	bool isKeyPressed = keyboardState[keycode] && !ImGui::GetIO().WantTextInput;
	// Draw Key
	ImGui_KeyboardKey(keycode, textureID, (bool)actionBinding, isKeyPressed);
	if (actionBinding) {
		// Name on hover
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text(actionBinding->action.ref.name.c_str());
			ImGui::EndTooltip();
		}
		// Right-click to delete
		if (LiveMode::IsOff()) {
			if (keycode != GLFW_KEY_UNKNOWN) {
				if (ImGui::BeginPopupContextItem("")) {
					if (ImGui::Button("Remove binding")) {
						setBinding(actionBinding, GLFW_KEY_UNKNOWN);
						m_presets.setPlaceholderPresetName();
					}
					ImGui::EndPopup();
				}
			}
		}
	}
	// Drag'n Drop !
	if (LiveMode::IsOff()) {
		if (actionBinding) {
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("KEYBOARD_KEY", &actionBinding, sizeof(ActionBinding*));
				ImGui_KeyboardKey(GLFW_KEY_UNKNOWN, textureID, (bool)actionBinding, false); // Preview
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
					setBinding(*((ActionBinding**)payload->Data), keycode);
				m_presets.setPlaceholderPresetName();
			}
			ImGui::EndDragDropTarget();
		}
	}
}

bool KeyBindings::ImGui_KeyboardKey(GLFW_Keycode keycode, unsigned int textureID, bool hasAnActionBound, bool isKeyPressed) {
	//
	ImGuiStyle& style = ImGui::GetStyle();
	//
	ImVec2 p = ImGui::GetCursorScreenPos();
	// Detect clic
	ImGui::PushID(keycode + 1294);
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
	float t = (SDL_GetTicks() - m_keyReleasedLastDate[keycode]) * 0.001f / ttl;
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
	const char* keyName = glfwGetKeyName(keycode, 0);
	if (keyName)
		draw_list->AddText(NULL, fontSize, ImVec2(p.x + KEY_SIZE * 0.5f - fontSize * 0.25f, p.y + KEY_SIZE * 0.5f - fontSize * 0.45f), col32text, keyName);

	ImGui::PopID();
	return is_active;
}

void KeyBindings::serializeBindings(const std::string& filepath) {
	std::vector<ActionRefAndBinding> boundActionsRefs;
	std::vector<ActionRef> actionsWithNoBinding;
	for (const ActionBinding& actionBinding : m_allActionsOwner) {
		if (actionBinding.keycode != GLFW_KEY_UNKNOWN)
			boundActionsRefs.emplace_back(actionBinding.action.ref, actionBinding.keycode);
		else
			actionsWithNoBinding.emplace_back(actionBinding.action.ref);
	}
	std::ofstream os(filepath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(boundActionsRefs),
			CEREAL_NVP(actionsWithNoBinding)
		);
	}
}

std::vector<ActionRef> KeyBindings::readBindingsFrom(const std::string& filepath) {
	std::vector<ActionRefAndBinding> boundActionsRefs;
	std::vector<ActionRef> actionsWithNoBinding;
	std::ifstream is(filepath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			CEREAL_NVP(boundActionsRefs),
			CEREAL_NVP(actionsWithNoBinding)
		);
	}
	for (const auto& actionRefAndBinding : boundActionsRefs) {
		if (ActionBinding* actionBinding = m_allActionsByType.tryFind(actionRefAndBinding.actionRef)) {
			setBinding(actionBinding, actionRefAndBinding.keycode);
		}
	}
	return actionsWithNoBinding;
}

const Action* KeyBindings::getActionByRef(const ActionRef& actionRef) const {
	for (const ActionBinding& actionBinding : m_allActionsOwner) {
		if (actionBinding.action.ref == actionRef)
			return &actionBinding.action;
	}
	return nullptr;
}