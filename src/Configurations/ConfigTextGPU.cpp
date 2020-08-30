#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include "Helper/DisplayInfos.h"
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
	m_computeShader.get().setUniform1f("u_aspectRatio", DisplayInfos::RenderTargetAspectRatio());
	m_computeShader.get().setUniform1i("u_nbLetters", m_letters.size());
	//
	m_computeShader.compute(particleSystem.getNbParticles());
	m_computeShader.get().unbind();
}

void ConfigTextGPU::uploadData() {
	m_lettersSSBO.uploadData(m_letters.size(), m_letters.data());
	m_offsetsSSBO.uploadData(m_offsets.size(), m_offsets.data());
}

bool ConfigTextGPU::setApplyAndRecord_AddOneChar(char keysym, StateModifier& stateModifer) {
	bool b = false;
	// Set
	if ('a' <= keysym && keysym <= 'z') {
		m_letters.push_back(keysym - 'a');
		m_offsets.push_back(m_offset);
		m_offset++;
		b = true;
	}
	else if (keysym == ' ') {
		m_offset++;
		b = true;
	}
	// Apply and record
	if (b) {
		uploadData();
		stateModifer.apply();
		stateModifer.recordChange({ StateChangeType::Text_AddChar, keysym });
	}
	return b;
}

void ConfigTextGPU::setApplyAndRecord_SupprOneChar(StateModifier& stateModifer) {
	// Set
	if (m_letters.size() > 0 && m_offsets.back() == m_offset - 1) { // last character is a letter
		m_letters.resize(m_letters.size() - 1);
		m_offsets.resize(m_offsets.size() - 1);
		m_offset--;
	}
	else if (m_offset > 0) { // last character is a space
		m_offset--;
	}
	// Apply and record
	uploadData();
	stateModifer.apply();
	stateModifer.recordChange({ StateChangeType::Text_SupprChar, '\0' }); // '\0' is just a random placeholder value because we need to submit one to recordChange
}

void ConfigTextGPU::setApplyAndRecord_SupprAllChars(StateModifier& stateModifer) {
	m_letters.resize(0);
	m_offsets.resize(0);
	m_offset = 0;
	uploadData();
	stateModifer.apply();
	stateModifer.recordChange({ StateChangeType::Text_SupprAll, '\0' }); // '\0' is just a random placeholder value because we need to submit one to recordChange
}