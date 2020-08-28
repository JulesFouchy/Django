#pragma once

#include "StateChange.h"

struct StateChangeTimestamp {
	StateChange stateChange;
	float time;

	StateChangeTimestamp(const StateChange& stateChange, float time)
		: stateChange(stateChange), time(time)
	{}
	StateChangeTimestamp() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(stateChange),
			CEREAL_NVP(time)
		);
	}
};