#pragma once

#include "ConfigType.h"

struct ConfigRef {
	ConfigRef() = default;
	ConfigRef(const std::string& name, ConfigType type)
		: name(name), type(type)
	{}

	std::string name;
	ConfigType type;

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