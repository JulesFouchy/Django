#pragma once

#include "ConfigType.h"

struct ConfigRef {
	ConfigRef() = default;
	ConfigRef(const std::string& mainName, const std::string& layoutName, ConfigType type)
		: mainName(mainName), layoutName(layoutName), type(type)
	{}
	ConfigRef(const std::string & name, ConfigType type)
		: ConfigRef(name, "", type)
	{}

	std::string mainName;
	std::string layoutName; // only for SHAPE-LAYOUT and SVG-LAYOUT
	ConfigType type;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(mainName),
			CEREAL_NVP(layoutName),
			CEREAL_NVP(type)
		);
	}
};