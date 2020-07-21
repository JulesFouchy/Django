#include "ConfigTextGPU.h"

ConfigTextGPU::ConfigTextGPU()
	: m_computeShader("internal-shaders/textConfig.comp"), m_text("")
{}

void ConfigTextGPU::applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) {

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