#include "ConfigGPU.h"

#include "Particles/ParticlesSystem.h"
#include "ConfigParams.h"

#include "Helper/File.h"
#include "Helper/DisplayInfos.h"

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
    m_computeShader.get().setUniform1f("u_aspectRatio", DisplayInfos::Ratio());
    m_computeShader.get().setUniform1i("u_count", params.count);
    m_computeShader.get().setUniform1i("u_intLR", params.intLR);
    m_computeShader.get().setUniform1i("u_intUD", params.intUD);
    m_computeShader.get().setUniform1f("u_wheel", params.wheel);
    m_computeShader.get().setUniform1f("u_ctrlWheel", params.ctrlWheel);
    m_computeShader.get().setUniform1f("u_shiftWheel", params.shiftWheel);
    m_computeShader.get().setUniform1f("u_altWheel", params.altWheel);
    m_computeShader.get().setUniform1f("u_seed", 100.31f);
    m_computeShader.get().setUniform2f("u_xySeed", glm::vec2(12, 154.51));
    m_computeShader.get().setUniform1f("a", 50.1);
    m_computeShader.get().setUniform2f("v", glm::vec2(0.564, 0.7));
    m_computeShader.get().setUniform1f("delta", 0.1657);
    m_computeShader.compute(particlesSystem.getNbParticles());
    m_computeShader.get().unbind();
}