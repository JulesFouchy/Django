#pragma once

#include "PhysicsSettings.h"
#include "VisualSettings.h"
#include "WindSettings.h"
#include "ParticleSystemSettings.h"

class Settings {
public:
	Settings() = default;
	~Settings() = default;

	void ImGuiWindows(ShaderPipeline& physicsCompute, ParticlesSystem& partSystem, Configuration& currentConfiguration);
	void apply(ShaderPipeline& physicsCompute, ParticlesSystem& partSystem, Configuration& currentConfiguration);

	void serializeTo(const std::string& filePath);
	void deserializeFrom(const std::string& filePath);

	inline PhysicsSettings&        getPhysics()    { return m_physicsSettings; }
	inline VisualSettings&         getVisuals()    { return m_visualSettings; }
	inline WindSettings&           getWind()       { return m_windSettings; }
	inline ParticleSystemSettings& getPartSystem() { return m_particleSystemSettings; }

private:
	PhysicsSettings m_physicsSettings;
	VisualSettings m_visualSettings;
	WindSettings m_windSettings;
	ParticleSystemSettings m_particleSystemSettings;
};