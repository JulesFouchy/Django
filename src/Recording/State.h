#pragma once

#include "Configurations/ConfigRef.h"

struct State {
	State() = default;

	ConfigRef configRef;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(configRef),
		);
	}
};