#pragma once

#include "Actions/ActionRef.h"
#include "Settings/AlphaTrailSettings.h"
#include "Settings/ColorSettings.h"
#include "Settings/ParticleSystemSettings.h"
#include "Settings/PhysicsSettings.h"
#include "Settings/WindSettings.h"
#include "Configurations/ConfigParams.h"
#include "Configurations/RandomParams.h"

struct State {
	State(float timestamp)
		: timestamp(timestamp)
	{}
	State() = default;

	float timestamp;
	ActionRef lastShape;
	ActionRef lastLayout;
	ActionRef currentAction;
	AlphaTrailSettingsValues alphaTrailValues;
	ColorSettingsValues colorValues;
	ParticleSystemSettingsValues particleSystemValues;
	PhysicsSettingsValues physicsValues;
	WindSettingsValues windValues;
	ConfigParams configParams;
	RandomParams randomParams;
	std::string textConfigValue;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(timestamp),
			CEREAL_NVP(lastShape),
			CEREAL_NVP(currentAction),
			CEREAL_NVP(alphaTrailValues),
			CEREAL_NVP(colorValues),
			CEREAL_NVP(particleSystemValues),
			CEREAL_NVP(physicsValues),
			CEREAL_NVP(windValues),
			CEREAL_NVP(configParams),
			CEREAL_NVP(randomParams),
			CEREAL_NVP(textConfigValue)
		);
	}
};