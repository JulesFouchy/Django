#pragma once

#include "ActionsByType.h"
#include "BindingsPresets.h"
#include "KeyboardLayout.h"
#include <list>

class StateModifier;

using GLFW_Keycode = int;

class KeyBindings {
	friend class BindingsPresets;
public:
	KeyBindings();
	~KeyBindings();

	const Action* getAction(GLFW_Keycode keycode);
	void addAction(Action action);
	void addAction(Action action, int type);
	void setupBindings(const std::string& presetFilepath, bool clearExistingBndings = true);
	void ImGui(StateModifier& stateModifier);
	inline void ImGuiWindow() { m_keyboardLayout.ImGuiWindow(); }
	void onKeyUp(int keycode);

	inline std::unordered_map<std::string, ActionBinding*>& getAllActionBindingsOfType(ActionType type) { return m_allActionsByType[type]; }
	
	const Action* getActionByRef(const ActionRef& actionRef) const;

	inline bool isSettingKeyboardLayout() { return m_keyboardLayout.isSettingKeyboardLayout(); }

private:
	void resetBindings();
	void setBinding(ActionBinding* actionBinding, GLFW_Keycode keycode);
	void swapBindings(ActionBinding* actionBinding1, ActionBinding* actionBinding2);
	void setupMiscellaneousBindings();
	void clearAllBindings();
	void serializeBindings(const std::string& filepath);
	std::vector<ActionRef> readBindingsFrom(const std::string& filepath);

	bool hasBinding(const ActionBinding* actionBinding);
	bool isKeyAvailable(GLFW_Keycode keycode);
	GLFW_Keycode findFirstFromLeft(const std::vector<GLFW_Keycode>& row);
	GLFW_Keycode findFirstFromRight(const std::vector<GLFW_Keycode>& row);

	void ImGui_KeyboardRow(const std::vector<GLFW_Keycode>& row, float indent, StateModifier& stateModifier);
	void ImGui_DragNDropKey(GLFW_Keycode keycode, ActionBinding* actionBinding = nullptr);
	bool ImGui_KeyboardKey(GLFW_Keycode keycode, unsigned int textureID, bool hasAnActionBound, bool isKeyPressed);

private:
	std::list<ActionBinding> m_allActionsOwner; // don't use a vector because all our pointers are invalidated when memory is reallocated
	ActionsByType m_allActionsByType;
	std::unordered_map<GLFW_Keycode, ActionBinding*> m_boundActions;
	Uint32* m_keyReleasedLastDate;
	bool m_mouseWasDraggingLastFrame = false;
	BindingsPresets m_presets;
	size_t m_nextAvailableKeyIdx = 0;
	KeyboardLayout m_keyboardLayout;
};
