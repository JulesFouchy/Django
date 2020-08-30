#pragma once

#include "Actions/ActionRef.h"
#include <variant>
#include <cereal/types/variant.hpp>

enum class StateChangeType {
	Action,
	AlphaTrail_Enabled,
	AlphaTrail_FixResiduals,
	AlphaTrail_Decay,
	AlphaTrail_Threshold,
	AlphaTrail_MinAlpha,
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
	Wind_Frequency,
	Wind_MinStrength,
	Wind_MaxStrength,
	Wind_Speed,
	Wind_Direction,
	ConfigParams,
	Param_Wheel,
	Random_Seed,
	Random_XYSeed
};

using StateChangeValue = std::variant<
	ActionRef,
	SDL_Scancode,
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