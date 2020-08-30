#pragma once

class StateModifier;

class MouseInteractions {
public:
	MouseInteractions() = default;
	~MouseInteractions() = default;

	void update(StateModifier& stateModifier);

	void setApplyAndRecord_ForceField(const glm::vec2& mousePos, StateModifier& stateModifier);
	void setApplyAndRecord_Burst(const glm::vec3& mousePosAndStrength, StateModifier& stateModifier);

private:
	std::queue<glm::vec2> m_forceFieldQueue;
	std::queue<glm::vec3> m_burstQueue;
};