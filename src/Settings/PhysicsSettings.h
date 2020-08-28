#pragma once

#include "Presets.h"

class StateModifier;

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

	void apply(StateModifier& stateModifier);
	void ImGui(StateModifier& stateModifier);

private:
	void setStiffnessInShader(StateModifier& stateModifier);
	void setDampingInShader(StateModifier& stateModifier);

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