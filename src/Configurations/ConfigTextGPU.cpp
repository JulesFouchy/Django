#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include "Viewports/Viewports.h"
#include "Helper/File.h"
#include "Helper/Input.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"

ConfigTextGPU::ConfigTextGPU()
	: Configuration("Text"), m_lettersSSBO(5, GL_DYNAMIC_READ), m_offsetsSSBO(6, GL_DYNAMIC_READ)
{
	std::string srcCode;
	MyFile::ToString("internal-shaders/config_Text.comp", &srcCode);
	m_computeShader.setSrcCode(srcCode);
	uploadData();
}

bool ConfigTextGPU::onKeyPressed(SDL_Scancode scancode, char keysym, StateModifier& stateModifier) {
	bool bHandled = false;
	if (m_bCaptureKeys) {
		if (scancode == SDL_SCANCODE_BACKSPACE) {
			if (Input::KeyIsDown(SDL_SCANCODE_LCTRL) || Input::KeyIsDown(SDL_SCANCODE_RCTRL)) {
				setApplyAndRecord_SupprAllChars(stateModifier);
			}
			else {
				setApplyAndRecord_SupprOneChar(stateModifier);
			}
			bHandled = true;
		}
		else {
			bHandled |= setApplyAndRecord_AddOneChar(keysym, stateModifier);
		}
	}
	return bHandled;
}

void ConfigTextGPU::applyTo(ParticleSystem& particleSystem, const ConfigParams& params, const RandomParams& randParams) {
	m_computeShader.get().bind();
	//
	m_computeShader.get().setUniform1i("u_NbOfParticles", particleSystem.getNbParticles());
	m_computeShader.get().setUniform1f("u_aspectRatio", Viewports::RenderArea.aspectRatio());
	m_computeShader.get().setUniform1i("u_nbLetters", m_nbLetters);
	//
	m_computeShader.compute(particleSystem.getNbParticles());
	m_computeShader.get().unbind();
}

void ConfigTextGPU::uploadData() {
	std::vector<int> letters;
	std::vector<int> offsets;
	int offset = 0;
	for (char keysym : m_text) {
		if ('a' <= keysym && keysym <= 'z') {
			letters.push_back(keysym - 'a');
			offsets.push_back(offset);
			offset++;
		}
		else if (keysym == ' ') {
			offset++;
		}
	}
	m_nbLetters = static_cast<int>(letters.size());

	m_lettersSSBO.uploadData(letters.size(), letters.data());
	m_offsetsSSBO.uploadData(offsets.size(), offsets.data());
}

bool ConfigTextGPU::setApplyAndRecord_AddOneChar(char keysym, StateModifier& stateModifer) {
	bool b = ('a' <= keysym && keysym <= 'z') || keysym == ' ';
	if (b) {
		m_text += keysym;
		uploadData();
		stateModifer.apply();
		stateModifer.recordChange({ StateChangeType::Text_AddChar, keysym });
	}
	return b;
}

void ConfigTextGPU::setApplyAndRecord_SupprOneChar(StateModifier& stateModifer) {
	if (m_text.size() > 0) {
		m_text.pop_back();
		uploadData();
		stateModifer.apply();
		stateModifer.recordChange({ StateChangeType::Text_SupprChar, '\0' }); // '\0' is just a random placeholder value because we need to submit one to recordChange
	}
}

void ConfigTextGPU::setApplyAndRecord_SupprAllChars(StateModifier& stateModifer) {
	m_text = "";
	uploadData();
	stateModifer.apply();
	stateModifer.recordChange({ StateChangeType::Text_SupprAll, '\0' }); // '\0' is just a random placeholder value because we need to submit one to recordChange
}