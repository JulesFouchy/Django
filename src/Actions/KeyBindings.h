#pragma once

#include "ActionsByType.h"
#include "BindingsPresets.h"
#include "KeyboardLayout.h"

class StateModifier;

class KeyBindings {
	friend class BindingsPresets;
public:
	KeyBindings();
	~KeyBindings();

	const Action* getAction(SDL_Scancode scancode);
	void addAction(Action action);
	void addAction(Action action, int type);
	void setupBindings(const std::string& presetFilepath, bool clearExistingBndings = true);
	void ImGui(StateModifier& stateModifier);
	void onKeyUp(SDL_Scancode scancode);

	inline std::unordered_map<std::string, ActionBinding*>& getAllActionBindingsOfType(ActionType type) { return m_allActionsByType[type]; }

	const Action* getActionByRef(const ActionRef& actionRef) const;

private:
	void resetBindings();
	void setBinding(ActionBinding* actionBinding, SDL_Scancode scancode);
	void swapBindings(ActionBinding* actionBinding1, ActionBinding* actionBinding2);
	void setupMiscellaneousBindings();
	void clearAllBindings();
	void serializeBindings(const std::string& filepath);
	std::vector<ActionRef> readBindingsFrom(const std::string& filepath);
	
	void startSettingKeyboardLayout();

	bool hasBinding(const ActionBinding* actionBinding);
	bool isKeyAvailable(SDL_Scancode scancode);
	SDL_Scancode findFirstFromLeft(std::vector<SDL_Scancode> row);
	SDL_Scancode findFirstFromRight(std::vector<SDL_Scancode> row);

	void ImGui_KeyboardRow(const std::vector<SDL_Scancode>& row, float indent, StateModifier& stateModifier);
	void ImGui_DragNDropKey(SDL_Scancode scancode, ActionBinding* actionBinding = nullptr);
	bool ImGui_KeyboardKey(SDL_Scancode scancode, unsigned int textureID, bool hasAnActionBound, bool isKeyPressed);

private:
	std::list<ActionBinding> m_allActionsOwner; // don't use a vector because all our pointers are invalidated when memory is reallocated
	ActionsByType m_allActionsByType;
	std::unordered_map<int, ActionBinding*> m_boundActions;
	Uint32* m_keyReleasedLastDate;
	bool m_mouseWasDraggingLastFrame = false;
	BindingsPresets m_presets;
	size_t m_nextAvailableKeyIdx = 0;
	KeyboardLayout m_keyboardLayout;
};