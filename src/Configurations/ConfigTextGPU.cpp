#include "ConfigTextGPU.h"

ConfigTextGPU::ConfigTextGPU()
	: m_computeShader("internal-shaders/textConfig.comp"), m_text("")
{}

void ConfigTextGPU::applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) {

}