#pragma once

#include "ActionType.h"

struct ActionRef {
	std::string name;
	ActionType type;

	ActionRef(const std::string& name, ActionType type)
		: name(name), type(type)
	{}
	ActionRef() = default;

	bool operator==(const ActionRef& o) const {
		return type == o.type && !name.compare(o.name);
	}

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(type)
		);
	}
};