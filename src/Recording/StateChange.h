#pragma once

#include "Actions/ActionRef.h"
#include "Settings/AlphaTrailSettings.h"
#include "Settings/ColorSettings.h"
#include "Settings/ParticleSystemSettings.h"
#include "Settings/PhysicsSettings.h"
#include "Settings/WindSettings.h"
#include <variant>

enum class StateChangeType {
	Action,
	AlphaTrail,
	Colors,
	Particles,
	Physics,
	Physics_Damping,
	Physics_Stiffness,
	Wind,
	Wind_NoiseFrequency,
	Wind_MinStrength,
	Wind_MaxStrength,
	Wind_DirectionAngle,
	Param_Wheel,
	Random_Seed,
	Random_XYSeed
};

using StateChangeValue = std::variant<
	ActionRef,
	AlphaTrailSettingsValues,
	ColorSettingsValues,
	ParticleSystemSettingsValues,
	PhysicsSettingsValues,
	WindSettingsValues,
	float,
	glm::vec2
>;

class StateChange {
public:
	StateChange() = default;
	StateChange(StateChangeType type, const StateChangeValue& value);

private:
	StateChangeType m_type;
	StateChangeValue m_value;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_type)
		);
	}
};