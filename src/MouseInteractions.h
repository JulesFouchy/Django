#pragma once

class StateModifier;

class MouseInteractions {
public:
	MouseInteractions() = default;
	~MouseInteractions() = default;

	void update(StateModifier& stateModifier);

	void setForceField(const glm::vec2& mousePos);
	void setBurst(const glm::vec3& mousePosAndStrength);

private:
	bool m_bForceField;
	glm::vec2 m_forceFieldPos;
	std::queue<glm::vec3> m_burstQueue;
};