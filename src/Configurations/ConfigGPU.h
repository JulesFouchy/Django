#pragma once

#include "OpenGL/ComputeShader.h"

class ParticlesSystem;

class ConfigGPU {
public:
	ConfigGPU() = default;
	ConfigGPU(const std::string& computeShaderSrcCode);
	ConfigGPU(ConfigGPU&& o) noexcept;
	~ConfigGPU() = default;
	void operator=(ConfigGPU&& o);

	void applyTo(ParticlesSystem& particlesSystem);

private:
	ComputeShader m_computeShader;
};