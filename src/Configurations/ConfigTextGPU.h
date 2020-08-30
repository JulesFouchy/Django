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

	bool setApplyAndRecord_AddOneChar(char keysym, StateModifier& stateModifier);
	void setApplyAndRecord_SupprOneChar (StateModifier& stateModifer);
	void setApplyAndRecord_SupprAllChars(StateModifier& stateModifer);

	inline const std::string& getText() const { return m_text; }
	inline void setText(const std::string& text) { m_text = text; uploadData(); }

private:
	void uploadData();

private:
	bool m_bCaptureKeys;
	std::string m_text;
	int m_nbLetters;

	ComputeShader m_computeShader;
	SSBO<int> m_lettersSSBO;
	SSBO<int> m_offsetsSSBO;
};