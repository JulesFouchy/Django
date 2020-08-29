#pragma once

#include "Configuration.h"
#include "OpenGL/ComputeShader.h"
#include "OpenGL/SSBO.h"

class StateModifier;

class ConfigTextGPU : public Configuration {
public:
	ConfigTextGPU();
	~ConfigTextGPU() = default;

	void applyTo(ParticleSystem& particleSystem, const ConfigParams& params, const RandomParams& randParams) override;
	bool onKeyPressed(SDL_Scancode scancode, char keysym, StateModifier& stateModifer);

	inline void setCaptureKeys(bool b) { m_bCaptureKeys = b; }
	inline void toggleCaptureKeys() { setCaptureKeys(!m_bCaptureKeys); }

private:
	void uploadData();

private:
	bool m_bCaptureKeys;
	std::vector<int> m_letters;
	std::vector<int> m_offsets;
	int m_offset = 0;

	ComputeShader m_computeShader;
	SSBO<int> m_lettersSSBO;
	SSBO<int> m_offsetsSSBO;
};