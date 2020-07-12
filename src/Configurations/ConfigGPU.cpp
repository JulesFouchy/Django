#include "ConfigGPU.h"

#include "Particles/ParticlesSystem.h"
#include "ConfigParams.h"
#include "RandomParams.h"

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

void ConfigGPU::applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) {
    m_computeShader.get().bind();
    //
    m_computeShader.get().setUniform1i("u_NbOfParticles", particlesSystem.getNbParticles());
    m_computeShader.get().setUniform1f("u_aspectRatio", DisplayInfos::Ratio());
    m_computeShader.get().setUniform1i("u_count", params.count);
    m_computeShader.get().setUniform1i("u_intLR", params.intLR);
    m_computeShader.get().setUniform1i("u_intUD", params.intUD);
    m_computeShader.get().setUniform1f("u_wheel", params.wheel);
    m_computeShader.get().setUniform1f("u_ctrlWheel", params.ctrlWheel);
    m_computeShader.get().setUniform1f("u_shiftWheel", params.shiftWheel);
    m_computeShader.get().setUniform1f("u_altWheel", params.altWheel);
    m_computeShader.get().setUniform1f("u_seed", randParams.seed);
    m_computeShader.get().setUniform2f("u_xySeed", randParams.xySeed);
    //
    m_computeShader.compute(particlesSystem.getNbParticles());
    m_computeShader.get().unbind();
}