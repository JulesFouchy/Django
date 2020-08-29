#pragma once

#include "Actions/ActionRef.h"
#include "Settings/AlphaTrailSettings.h"
#include "Settings/ColorSettings.h"
#include "Settings/ParticleSystemSettings.h"
#include "Settings/PhysicsSettings.h"
#include "Settings/WindSettings.h"
#include <variant>
#include <cereal/types/variant.hpp>

enum class StateChangeType {
	Action,
	AlphaTrail,
	Colors,
	Particles,
	ConfigParams,
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
	SDL_Scancode,
	AlphaTrailSettingsValues,
	ColorSettingsValues,
	ParticleSystemSettingsValues,
	PhysicsSettingsValues,
	WindSettingsValues,
	float,
	glm::vec2
>;

struct StateChange {
public:
	StateChange() = default;
	StateChange(StateChangeType type, const StateChangeValue& value)
		: type(type), value(value)
	{}

	StateChangeType type;
	StateChangeValue value;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(type),
			CEREAL_NVP(value)
		);
	}
};