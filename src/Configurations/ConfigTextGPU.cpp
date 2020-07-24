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
	//
	m_computeShader.compute(particlesSystem.getNbParticles());
	m_computeShader.get().unbind();
}

bool ConfigTextGPU::onKeyPressed(SDL_KeyboardEvent keyEvent) {
	bool bHandled = false;
	if (m_bCaptureKeys) {
		if (keyEvent.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
			if (m_text.size() > 0)
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