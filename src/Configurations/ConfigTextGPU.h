#pragma once

#include "Configuration.h"
#include "OpenGL/ComputeShader.h"

class ConfigTextGPU : public Configuration {
public:
	ConfigTextGPU();
	~ConfigTextGPU() = default;

	void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) override;

private:
	ComputeShader m_computeShader;
	std::string m_text;
};