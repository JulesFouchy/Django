#pragma once

#include "Presets.h"

class ShaderPipeline;
struct PhysicsSettingsValues {
	// en.wikipedia.org/wiki/Harmonic_oscillator#Damped_harmonic_oscillator
	// High Level parameters
	float pulsation = 4.472f;
	float dampingRatio = 0.335f;

	inline const float getStiffness() { return m_stiffness; }
	inline const float getDamping()   { return m_damping; }
private:
	// actual physical parameters
	float m_stiffness;
	float m_damping;

public:
	PhysicsSettingsValues() {
		computePhysicalParameters();
	}
	inline void computePhysicalParameters() {
		computeStiffness();
		computeDamping();
	}
	inline void computeStiffness() {
		m_stiffness = pulsation * pulsation;
	}
	inline void computeDamping() {
		m_damping = 2 * dampingRatio * pulsation;
	}
	//inline void computeHighLevelParameters() {
	//	computePulsation();
	//	computeDampingRatio();
	//}
	//inline void computePulsation() {
	//	pulsation = sqrt(stiffness);
	//}
	//inline void computeDampingRatio() {
	//	dampingRatio = damping * 0.5 / sqrt(stiffness);
	//}

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(pulsation),
			CEREAL_NVP(dampingRatio)
		);
		computePhysicalParameters();
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
			CEREAL_NVP(m_values),
			CEREAL_NVP(m_presets)
		);
	}
};