#pragma once

#include "Configuration.h"
#include "OpenGL/ComputeShader.h"

class ConfigGPU : public Configuration {
	friend class ConfigSVGManager;
public:
	ConfigGPU() = default;
	~ConfigGPU() = default;
	ConfigGPU(ConfigGPU&& o) noexcept;
	void operator=(ConfigGPU&& o);

	void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) override;

	void initWithSrcCode(const std::string& computeShaderSrcCode);
	void initWithFilePath(const std::string& computeShaderFilePath);

private:
	ComputeShader m_computeShader;
};