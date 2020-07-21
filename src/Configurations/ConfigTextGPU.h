#pragma once

#include "Configuration.h"
#include "OpenGL/ComputeShader.h"

class ConfigTextGPU : public Configuration {
public:
	ConfigTextGPU();
	~ConfigTextGPU() = default;

	void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) override;
	bool onKeyPressed(SDL_KeyboardEvent keyEvent);

	inline void setCaptureKeys(bool b) { m_bCaptureKeys = b; }
	inline void toggleCaptureKeys() { setCaptureKeys(!m_bCaptureKeys); }

private:
	ComputeShader m_computeShader;
	std::string m_text;
	bool m_bCaptureKeys;
};