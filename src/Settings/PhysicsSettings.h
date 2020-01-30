#pragma once

class PhysicsSettings {
public:
	PhysicsSettings(float stiffness = 20.0f, float damping = 3.0f);
	~PhysicsSettings() = default;

	void setUniforms();
	void ImGui_Parameters();

private:
	void setStiffnessInShader();
	void setDampingInShader();

private:
	float m_stiffness;
	float m_damping;
};