#pragma once

#include "Actions/ActionRef.h"

struct ActionTimestamp {
	ActionRef actionRef;
	float time;

	ActionTimestamp(const ActionRef& actionRef, float time)
		: actionRef(actionRef), time(time)
	{}
	ActionTimestamp() = default;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(actionRef),
			CEREAL_NVP(time)
		);
	}
};