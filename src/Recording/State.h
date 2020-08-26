#pragma once

#include "Configurations/ConfigRef.h"

struct State {
	State(float timestamp, const ConfigRef& configRef)
		: timestamp(timestamp), configRef(configRef)
	{}
	State() = default;

	float timestamp;
	ConfigRef configRef;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(timestamp),
			CEREAL_NVP(configRef)
		);
	}
};