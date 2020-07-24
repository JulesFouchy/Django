#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include "Helper/DisplayInfos.h"
#include "Helper/File.h"

ConfigTextGPU::ConfigTextGPU()
	: m_text("")
{
	MyFile::ToString("internal-shaders/textConfig.comp", &m_srcCodeBase);
	updateSrcCode();
}

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
	if (bHandled)
		updateSrcCode();
	return bHandled;
}

void ConfigTextGPU::updateSrcCode() {
	std::string srcCode = R"V0G0N(
#version 430
const float radius = 0.1;
const uint nbLetters = 5;
const float offsets[nbLetters] = {0, 1, 2, 4, 5};
const int letters[nbLetters] = {0, 1, 2, 3, 3};
)V0G0N"
+ m_srcCodeBase;
	spdlog::info(srcCode);
	m_computeShader.setSrcCode(srcCode);
}