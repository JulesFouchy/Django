#pragma once

class ShaderPipeline;

class PhysicsSettings {
public:
	PhysicsSettings(float stiffness = 20.0f, float damping = 3.0f);
	~PhysicsSettings() = default;

	void setUniforms(ShaderPipeline& shader);
	void ImGui_Parameters(ShaderPipeline& shader);

private:
	void setStiffnessInShader(ShaderPipeline& shader);
	void setDampingInShader(ShaderPipeline& shader);

private:
	float m_stiffness;
	float m_damping;
};