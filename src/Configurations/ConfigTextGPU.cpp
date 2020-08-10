#include "ConfigTextGPU.h"

#include "Particles/ParticlesSystem.h"
#include "Helper/DisplayInfos.h"
#include "Helper/File.h"
#include "Helper/ArrayStringified.h"

ConfigTextGPU::ConfigTextGPU()
	: m_text("")
{
	MyFile::ToString("internal-shaders/textConfigTemplate.comp", &m_srcCodeBase);
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

bool ConfigTextGPU::onKeyPressed(SDL_Scancode scancode, char keysym) {
	bool bHandled = false;
	if (m_bCaptureKeys) {
		if (scancode == SDL_SCANCODE_BACKSPACE) {
			if (m_text.size() > 0)
				m_text.erase(m_text.size() - 1, 1);
			bHandled = true;
		}
		else {
			if (('a' <= keysym && keysym <= 'z') || keysym == ' ') {
				m_text += keysym;
				bHandled = true;
			}
		}
	}
	if (bHandled)
		updateSrcCode();
	return bHandled;
}

void ConfigTextGPU::updateSrcCode() {
	int offset = 0;
	int nbLetters = 0;
	ArrayStringified offsets;
	ArrayStringified letters;
	if (m_text.empty()) {
		nbLetters = 1;
		offsets.push("0");
		letters.push("-1");
	}
	else {
		for (char c : m_text) {
			if ('a' <= c && c <= 'z') {
				nbLetters++;
				offsets.push(std::to_string(offset));
				letters.push(std::to_string((int)(c - 'a')));
			}
			offset++;
		}
	}
	offsets.close();
	letters.close();
	//
	std::string srcCode =
"#version 430 \n"
"const float letterWidth  = 0.1; \n"
"const float letterHeight = 0.3; \n"
"const uint nbLetters = " + std::to_string(nbLetters) + ";\n"
"const float offsets[nbLetters] = "+ offsets.string() +";\n"
"const int letters[nbLetters] = "+ letters.string() +";\n"
+ m_srcCodeBase;
	m_computeShader.setSrcCode(srcCode);
}