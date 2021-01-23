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
	void ImGuiMainWindow(StateModifier& stateModifier);
	void ImGuiOpenWindowsCheckboxes();
	void applyAndRecord(StateModifier& stateModifier);

	void serializeTo(const std::string& filePath);
	void deserializeFrom(const std::string& filePath);

	inline PhysicsSettings&        physics()    { return m_physicsSettings; }
	inline AlphaTrailSettings&     alphaTrail() { return m_alphaTrailSettings; }
	inline ColorSettings&          colors()     { return m_colorSettings; }
	inline WindSettings&           wind()       { return m_windSettings; }
	inline ParticleSystemSettings& partSystem() { return m_particleSystemSettings; }

private:
	PhysicsSettings m_physicsSettings;
	AlphaTrailSettings m_alphaTrailSettings;
	ColorSettings m_colorSettings;
	WindSettings m_windSettings;
	ParticleSystemSettings m_particleSystemSettings;

	bool m_bOpenPhysics = true;
	bool m_bOpenTrail = true;
	bool m_bOpenColors = true;
	bool m_bOpenWind = true;
	bool m_bOpenParticles = true;
};