#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include "Helper/DisplayInfos.h"
#include "Helper/File.h"
#include "Helper/Input.h"

ConfigTextGPU::ConfigTextGPU()
	: Configuration("Text"), m_lettersSSBO(5, GL_DYNAMIC_READ), m_offsetsSSBO(6, GL_DYNAMIC_READ)
{
	std::string srcCode;
	MyFile::ToString("internal-shaders/config_Text.comp", &srcCode);
	m_computeShader.setSrcCode(srcCode);
	uploadData();
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

bool ConfigTextGPU::onKeyPressed(SDL_Scancode scancode, char keysym, StateModifier& stateModifer) {
	bool bHandled = false;
	if (m_bCaptureKeys) {
		if (scancode == SDL_SCANCODE_BACKSPACE) {
			if (Input::KeyIsDown(SDL_SCANCODE_LCTRL) || Input::KeyIsDown(SDL_SCANCODE_RCTRL)) { // clear everything
				m_letters.resize(0);
				m_offsets.resize(0);
				m_offset = 0;
			}
			else {
				if (m_letters.size() > 0 && m_offsets.back() == m_offset - 1) { // last character is a letter
					m_letters.resize(m_letters.size() - 1);
					m_offsets.resize(m_offsets.size() - 1);
					m_offset--;
				}
				else if (m_offset > 0) { // last character is a space
					m_offset--;
				}
			}
			bHandled = true;
		}
		else {
			if ('a' <= keysym && keysym <= 'z') {
				m_letters.push_back(keysym - 'a');
				m_offsets.push_back(m_offset);
				m_offset++;
				bHandled = true;
			}
			else if (keysym == ' ') {
				m_offset++;
				bHandled = true;
			}
		}
	}
	if (bHandled)
		uploadData();
	return bHandled;
}

void ConfigTextGPU::uploadData() {
	m_lettersSSBO.uploadData(m_letters.size(), m_letters.data());
	m_offsetsSSBO.uploadData(m_offsets.size(), m_offsets.data());
}