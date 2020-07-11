#include "ConfigGPU.h"

#include "Particles/ParticlesSystem.h"

ConfigGPU::ConfigGPU(const std::string& computeShaderSrcCode) {
    spdlog::info(computeShaderSrcCode);
    m_computeShader.initWithCode(computeShaderSrcCode);
    Log::separationLine();
}
ConfigGPU::ConfigGPU(ConfigGPU&& o) noexcept
    : m_computeShader(std::move(o.m_computeShader))
{}
void ConfigGPU::operator=(ConfigGPU&& o) {
    m_computeShader = std::move(o.m_computeShader);
}

void ConfigGPU::applyTo(ParticlesSystem& particlesSystem) {
    m_computeShader.get().bind();
    m_computeShader.get().setUniform1i("u_NbOfParticles", particlesSystem.getNbParticles());
    m_computeShader.get().setUniform1i("u_NbClusters", 3);
    m_computeShader.get().setUniform1f("u_Radius", 0.8);
    m_computeShader.get().setUniform1f("u_Seed", 0);
    m_computeShader.get().setUniform1f("a", 0.1);
    m_computeShader.get().setUniform2f("v", glm::vec2(0.8, 0.7));
    m_computeShader.get().setUniform2f("xyOff", glm::vec2(12, 14.51));
    m_computeShader.get().setUniform1f("delta", 2);
    m_computeShader.compute(particlesSystem.getNbParticles());
    m_computeShader.get().unbind();
}