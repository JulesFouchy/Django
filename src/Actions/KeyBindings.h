#pragma once

#include "ActionsByType.h"
#include "BindingsPresets.h"

class ConfigManager;
class ParticlesSystem;

struct ActionIdentifier {
	std::string name;
	ActionType type;

	ActionIdentifier(const std::string& name, ActionType type)
		: name(name), type(type)
	{}
	ActionIdentifier() = default;

	bool operator==(const ActionIdentifier& o) {
		return type == o.type && !name.compare(o.name);
	}

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(type)
		);
	}
};

class KeyBindings {
	friend class BindingsPresets;
public:
	KeyBindings();
	~KeyBindings();

	const Action* getAction(SDL_Scancode scancode);
	void addAction(Action action);
	void addAction(Action action, int type);
	void setupBindings(const std::string& presetFilepath, bool clearExistingBndings = true);
	void ImGui(ConfigManager& configManager, ParticlesSystem& partSystem);
	void onKeyUp(SDL_Scancode scancode);

	inline std::unordered_map<std::string, ActionBinding*>& getAllActionBindingsOfType(ActionType type) { return m_allActionsByType[type]; }

private:
	void resetBindings();
	void setBinding(ActionBinding* actionBinding, SDL_Scancode scancode);
	void swapBindings(ActionBinding* actionBinding1, ActionBinding* actionBinding2);
	void setupMiscellaneousBindings();
	void clearAllBindings();
	void serializeBindings(const std::string& filepath);
	std::vector<ActionIdentifier> readBindingsFrom(const std::string& filepath);

	bool hasBinding(const ActionBinding* actionBinding);
	bool isKeyAvailable(SDL_Scancode scancode);
	SDL_Scancode findFirstFromLeft(std::vector<SDL_Scancode> row);
	SDL_Scancode findFirstFromRight(std::vector<SDL_Scancode> row);

	void ImGui_KeyboardRow(ConfigManager& configManager, ParticlesSystem& partSystem, const std::vector<SDL_Scancode>& row, float indent);
	void ImGui_DragNDropKey(SDL_Scancode scancode, ActionBinding* actionBinding = nullptr);
	bool ImGui_KeyboardKey(SDL_Scancode scancode, unsigned int textureID, bool hasAnActionBound, bool isKeyPressed);

private:
	std::list<ActionBinding> m_allActionsOwner; // don't use a vector because all our pointers are invalidated when memory is reallocated
	ActionsByType m_allActionsByType;
	std::unordered_map<int, ActionBinding*> m_boundActions;
	Uint32* m_keyReleasedLastDate;
	BindingsPresets m_presets;
	size_t nextAvailableKeyIdx = 0;
	std::vector<SDL_Scancode> allKeys;
	std::vector<SDL_Scancode> firstRow = {
		SDL_SCANCODE_Q,
		SDL_SCANCODE_W,
		SDL_SCANCODE_E,
		SDL_SCANCODE_R,
		SDL_SCANCODE_T,
		SDL_SCANCODE_Y,
		SDL_SCANCODE_U,
		SDL_SCANCODE_I,
		SDL_SCANCODE_O,
		SDL_SCANCODE_P,
		SDL_SCANCODE_LEFTBRACKET,
		SDL_SCANCODE_RIGHTBRACKET,
		SDL_SCANCODE_BACKSLASH,
	};
	std::vector<SDL_Scancode> secondRow = {
		SDL_SCANCODE_A,
		SDL_SCANCODE_S,
		SDL_SCANCODE_D,
		SDL_SCANCODE_F,
		SDL_SCANCODE_G,
		SDL_SCANCODE_H,
		SDL_SCANCODE_J,
		SDL_SCANCODE_K,
		SDL_SCANCODE_L,
		SDL_SCANCODE_SEMICOLON,
		SDL_SCANCODE_APOSTROPHE,
	};
	std::vector<SDL_Scancode> thirdRow = {
		SDL_SCANCODE_Z,
		SDL_SCANCODE_X,
		SDL_SCANCODE_C,
		SDL_SCANCODE_V,
		SDL_SCANCODE_B,
		SDL_SCANCODE_N,
		SDL_SCANCODE_M,
		SDL_SCANCODE_COMMA,
		SDL_SCANCODE_PERIOD,
		SDL_SCANCODE_SLASH,
	};
};