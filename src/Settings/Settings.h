#pragma once

#include "PhysicsSettings.h"
#include "AlphaTrailSettings.h"
#include "ColorSettings.h"
#include "WindSettings.h"
#include "ParticleSystemSettings.h"

class Settings {
public:
	Settings() = default;
	~Settings() = default;

	void ImGuiWindows(StateModifier& stateModifier);
	void apply(StateModifier& stateModifier);

	void serializeTo(const std::string& filePath);
	void deserializeFrom(const std::string& filePath);

	inline PhysicsSettings&        getPhysics()    { return m_physicsSettings; }
	inline AlphaTrailSettings&     getTrail()      { return m_alphaTrailSettings; }
	inline ColorSettings&          getColors()     { return m_colorSettings; }
	inline WindSettings&           getWind()       { return m_windSettings; }
	inline ParticleSystemSettings& getPartSystem() { return m_particleSystemSettings; }

private:
	PhysicsSettings m_physicsSettings;
	AlphaTrailSettings m_alphaTrailSettings;
	ColorSettings m_colorSettings;
	WindSettings m_windSettings;
	ParticleSystemSettings m_particleSystemSettings;
};