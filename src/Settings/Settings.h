#pragma once

#include "PhysicsSettings.h"
#include "VisualSettings.h"
#include "WindSettings.h"

class Settings {
public:
	Settings();
	~Settings() = default;

	void ImGuiWindows(ShaderPipeline& physicsCompute);

	void serializeTo(const std::string& filePath);

	inline PhysicsSettings& getPhysics() { return m_physicsSettings; }
	inline VisualSettings&  getVisuals() { return m_visualSettings; }
	inline WindSettings&    getWind()    { return m_windSettings; }

private:
	PhysicsSettings m_physicsSettings;
	VisualSettings m_visualSettings;
	WindSettings m_windSettings;
};