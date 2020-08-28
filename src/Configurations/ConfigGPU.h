#pragma once

#include "Configuration.h"
#include "OpenGL/ComputeShader.h"

class ConfigGPU : public Configuration {
	friend class ConfigSVGManager;
public:
	ConfigGPU() = default;
	ConfigGPU(const std::string& mainName, const std::string& layoutName)
		: Configuration(mainName, layoutName)
	{}
	ConfigGPU(const std::string& name)
		: ConfigGPU(name, "")
	{}
	~ConfigGPU() = default;
	ConfigGPU(ConfigGPU&& o) noexcept;
	void operator=(ConfigGPU&& o);

	void applyTo(ParticleSystem& particleSystem, const ConfigParams& params, const RandomParams& randParams) override;

	void initWithSrcCode(const std::string& computeShaderSrcCode);
	void initWithFilePath(const std::string& computeShaderFilePath);

private:
	ComputeShader m_computeShader;
};