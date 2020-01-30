#pragma once

class ShaderPipeline;

class PhysicsSettings {
public:
	PhysicsSettings(ShaderPipeline& physicsShader, float stiffness = 20.0f, float damping = 3.0f);
	~PhysicsSettings() = default;

	void setUniforms();
	void ImGui_Parameters();

private:
	void setStiffnessInShader();
	void setDampingInShader();

private:
	float m_stiffness;
	float m_damping;
	ShaderPipeline& m_physicsShader;
};