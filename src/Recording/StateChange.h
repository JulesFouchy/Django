#pragma once

#include "Actions/ActionRef.h"
#include "Settings/AlphaTrailSettings.h"
#include "Settings/WindSettings.h"
#include <variant>
#include <cereal/types/variant.hpp>

enum class StateChangeType {
	Action,
	AlphaTrail,
	Color_Background,
	Color_GradientMode,
	Color_HueStart,
	Color_HueEnd,
	Color_Saturation,
	Color_Value,
	Color_ColorStart,
	Color_ColorEnd,
	Particles_Number,
	Particles_Radius,
	Physics_Pulsation,
	Physics_DampingRatio,
	ConfigParams,
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
	WindSettingsValues,
	unsigned int,
	bool,
	float,
	glm::vec2,
	glm::vec3
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