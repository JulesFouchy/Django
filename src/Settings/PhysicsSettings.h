#pragma once

#include "Presets.h"

class ShaderPipeline;

struct PhysicsSettingsValues {
	float stiffness = 20.0f;
	float damping = 3.0f;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(stiffness),
			CEREAL_NVP(damping)
		);
	}
};

class PhysicsSettings {
public:
	PhysicsSettings();
	~PhysicsSettings() = default;

	void apply(ShaderPipeline& physicsCompute);
	void ImGui(ShaderPipeline& physicsCompute);

private:
	void setStiffnessInShader(ShaderPipeline& physicsCompute);
	void setDampingInShader(ShaderPipeline& physicsCompute);

private:
	PhysicsSettingsValues m_values;
	Presets<PhysicsSettingsValues> m_presets;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_values)
		);
	}
};