#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include "Helper/DisplayInfos.h"

ConfigTextGPU::ConfigTextGPU()
	: m_computeShader("internal-shaders/textConfig.comp"), m_text("")
{}

void ConfigTextGPU::applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) {
	m_computeShader.get().bind();
	//
	m_computeShader.get().setUniform1i("u_NbOfParticles", particlesSystem.getNbParticles());
	m_computeShader.get().setUniform1f("u_aspectRatio", DisplayInfos::Ratio());
	m_computeShader.get().setUniform1i("u_textEncoded", computeTextCode());
	m_computeShader.get().setUniform1i("u_nbLetters", m_text.size()); // TODO subtract the nb of spaces
	//
	m_computeShader.compute(particlesSystem.getNbParticles());
	m_computeShader.get().unbind();
}

bool ConfigTextGPU::onKeyPressed(SDL_KeyboardEvent keyEvent) {
	bool bHandled = false;
	if (m_bCaptureKeys) {
		if (keyEvent.keysym.scancode == SDL_SCANCODE_BACKSPACE && m_text.size() > 0) {
			m_text.erase(m_text.size() - 1, 1);
			bHandled = true;
		}
		else {
			char key = keyEvent.keysym.sym;
			if (('a' <= key && key <= 'z') || key == ' ') {
				m_text += key;
				bHandled = true;
			}
		}
	}
	return m_bCaptureKeys;
}

int ConfigTextGPU::computeTextCode() {
	if (m_text.size() == 0)
		return -1;
	int code = 0;
	int pow = 1;
	for (char c : m_text) {
		int charCode;
		if (c == ' ') {
			charCode = 26;
		}
		else {
			charCode = c - 'a';
		}
		code += charCode * pow;
		pow *= 27;
	}
	return code;
}