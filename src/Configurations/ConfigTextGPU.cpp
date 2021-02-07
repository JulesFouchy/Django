#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include <Cool/App/RenderState.h>
#include "Helper/File.h"
#include <Cool/App/Input.h>
#include "StateModifier.h"
#include "Recording/StateChange.h"

using namespace Cool;

ConfigTextGPU::ConfigTextGPU()
	: Configuration("Text"), m_lettersSSBO(5, GL_DYNAMIC_READ), m_offsetsSSBO(6, GL_DYNAMIC_READ)
{
	std::string srcCode;
	MyFile::ToString("internal-shaders/config_Text.comp", &srcCode);
	m_computeShader.setSrcCode(srcCode);
	uploadData();
}

bool ConfigTextGPU::onKeyPressed(int keycode, int mods, StateModifier& stateModifier) {
	bool bHandled = false;
	if (m_bCaptureKeys) {
		if (keycode == GLFW_KEY_BACKSPACE) {
			if (mods & GLFW_MOD_CONTROL) {
				setApplyAndRecord_SupprAllChars(stateModifier);
			}
			else {
				setApplyAndRecord_SupprOneChar(stateModifier);
			}
			bHandled = true;
		}
		else {
			bHandled |= setApplyAndRecord_AddOneChar(keycode, stateModifier);
		}
	}
	return bHandled;
}

void ConfigTextGPU::applyTo(ParticleSystem& particleSystem, const ConfigParams& params, const RandomParams& randParams) {
	m_computeShader.get().bind();
	//
	m_computeShader.get().setUniform1i("u_NbOfParticles", particleSystem.getNbParticles());
	m_computeShader.get().setUniform1f("u_aspectRatio", RenderState::Size().aspectRatio());
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

bool ConfigTextGPU::setApplyAndRecord_AddOneChar(int keycode, StateModifier& stateModifer) {
	const char* str = glfwGetKeyName(keycode, 0);
	char keysym;
	if (strlen(str) == 1)
		keysym = str[0];
	else
		return false;
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