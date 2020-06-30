#pragma once

class ShaderPipeline;

class PhysicsSettings {
public:
	PhysicsSettings() = default;
	~PhysicsSettings() = default;

	void apply(ShaderPipeline& physicsCompute);
	void ImGui(ShaderPipeline& physicsCompute);

private:
	void setStiffnessInShader(ShaderPipeline& physicsCompute);
	void setDampingInShader(ShaderPipeline& physicsCompute);

private:
	float m_stiffness = 20.0f;
	float m_damping = 3.0f;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_stiffness),
			CEREAL_NVP(m_damping)
		);
	}
};