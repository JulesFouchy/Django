#pragma once

class ShaderPipeline;

class PhysicsSettings {
public:
	PhysicsSettings(ShaderPipeline& physicsShader, float raideur = 20.0f, float dampening = 3.0f);
	~PhysicsSettings() = default;

	void setUniforms();
	void ImGui_Parameters();

private:
	void setRaideurInShader();
	void setDampeningInShader();

private:
	float m_raideur;
	float m_damping;
	ShaderPipeline& m_physicsShader;
};