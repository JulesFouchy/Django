#pragma once

#include "OpenGL/ComputeShader.h"

class ParticlesSystem;
struct ConfigParams;

class ConfigGPU {
public:
	ConfigGPU() = default;
	~ConfigGPU() = default;
	ConfigGPU(ConfigGPU&& o) noexcept;
	void operator=(ConfigGPU&& o);

	void initWithSrcCode(const std::string& computeShaderSrcCode);
	void initWithFilePath(const std::string& computeShaderFilePath);

	void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params);

private:
	ComputeShader m_computeShader;
};