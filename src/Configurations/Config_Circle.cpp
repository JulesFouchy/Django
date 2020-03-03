#include "Config_Circle.h"

#include "Particles/ParticlesSystem.h"

#include "Debugging/Log.h"

#include "Helper/DisplayInfos.h"

#include <imgui/imgui.h>

Config_Circle::Config_Circle()
	: Configuration("Circle"), 
	  m_radius(0.8f), m_rotation(0.0f)
{}

bool Config_Circle::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		m_unitCircle.resize(nbParticles);
		for (int i = 0; i < nbParticles; ++i) {
			float angle = 2.0f * 3.1415927f * i / (float)nbParticles;
			m_unitCircle[i].x = cos(angle) / DisplayInfos::Ratio();
			m_unitCircle[i].y = sin(angle);
		}
		return true;
	}
	return false;
}

void Config_Circle::applyTo(ParticlesSystem& partSystem) {
	int offset = floor(m_rotation / 6.2832f * partSystem.size());
	for (int i = 0; i < partSystem.size(); ++i)
		partSystem[i] = m_radius * m_unitCircle[(i + offset) % partSystem.size()];
	partSystem.sendRestPositionsToGPU();
}

void Config_Circle::ImGuiParameters(ParticlesSystem& particlesSystem) {
	bool change =
		ImGui::SliderFloat("Radius", &m_radius, 0.0f, 1.0f) ||
		ImGui::DragFloat("Rotation", &m_rotation, 0.3f)
	;
	if (change)
		applyTo(particlesSystem);
}