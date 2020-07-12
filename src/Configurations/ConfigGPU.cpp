#include "ConfigGPU.h"

#include "Particles/ParticlesSystem.h"
#include "ConfigParams.h"

#include "Helper/File.h"

ConfigGPU::ConfigGPU(ConfigGPU&& o) noexcept
    : m_computeShader(std::move(o.m_computeShader))
{}
void ConfigGPU::operator=(ConfigGPU&& o) {
    m_computeShader = std::move(o.m_computeShader);
}

void ConfigGPU::initWithSrcCode(const std::string& computeShaderSrcCode) {
    // spdlog::info(computeShaderSrcCode);
    m_computeShader.initWithCode(computeShaderSrcCode);
    // Log::separationLine();
}
void ConfigGPU::initWithFilePath(const std::string& computeShaderFilePath) {
    std::string src;
    MyFile::ToString(computeShaderFilePath, &src);
    initWithSrcCode(src);
}

void ConfigGPU::applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params) {
    m_computeShader.get().bind();
    m_computeShader.get().setUniform1i("u_NbOfParticles", particlesSystem.getNbParticles());
    m_computeShader.get().setUniform1i("u_NbClusters", params.count);
    m_computeShader.get().setUniform1f("u_Radius", params.wheel);
    m_computeShader.get().setUniform1f("u_Seed", 100.165);
    m_computeShader.get().setUniform1f("a", 50.1);
    m_computeShader.get().setUniform2f("v", glm::vec2(0.564, 0.7));
    m_computeShader.get().setUniform2f("xyOff", glm::vec2(12, 154.51));
    m_computeShader.get().setUniform1f("delta", 0.1657);
    m_computeShader.compute(particlesSystem.getNbParticles());
    m_computeShader.get().unbind();
}