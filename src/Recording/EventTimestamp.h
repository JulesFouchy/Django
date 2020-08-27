#pragma once

#include "Event.h"

struct EventTimestamp {
	Event event;
	float time;

	EventTimestamp(const Event& event, float time)
		: event(event), time(time)
	{}
	EventTimestamp() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(event),
			CEREAL_NVP(time)
		);
	}
};