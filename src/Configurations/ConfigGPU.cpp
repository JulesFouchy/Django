#include "ConfigGPU.h"

#include "Particles/ParticlesSystem.h"
#include "ConfigParams.h"
#include "RandomParams.h"

#include "Helper/File.h"
#include <Cool/App/RenderState.h>

using namespace Cool;

ConfigGPU::ConfigGPU(ConfigGPU&& o) noexcept
    : m_computeShader(std::move(o.m_computeShader))
{}
void ConfigGPU::operator=(ConfigGPU&& o) {
    m_computeShader = std::move(o.m_computeShader);
}

void ConfigGPU::initWithSrcCode(const std::string& computeShaderSrcCode) {
    // spdlog::info(computeShaderSrcCode);
    m_computeShader.setSrcCode(computeShaderSrcCode);
    // Log::separationLine();
}
void ConfigGPU::initWithFilePath(const std::string& computeShaderFilePath) {
    std::string src;
    MyFile::ToString(computeShaderFilePath, &src);
    initWithSrcCode(src);
}

void ConfigGPU::applyTo(ParticleSystem& particleSystem, const ConfigParams& params, const RandomParams& randParams) {
    m_computeShader.get().bind();
    //
    m_computeShader.get().setUniform1i("u_NbOfParticles", particleSystem.getNbParticles());
    m_computeShader.get().setUniform1f("u_aspectRatio", RenderState::Size().aspectRatio());
    m_computeShader.get().setUniform1i("u_LR", params.getLR());
    m_computeShader.get().setUniform1i("u_UD", params.getUD());
    m_computeShader.get().setUniform1i("u_wheel", params.getWheel());
    m_computeShader.get().setUniform1i("u_ctrlWheel", params.getCtrlWheel());
    m_computeShader.get().setUniform1i("u_shiftWheel", params.getShiftWheel());
    m_computeShader.get().setUniform1i("u_altWheel", params.getAltWheel());
    m_computeShader.get().setUniform1f("u_seed", randParams.seed);
    m_computeShader.get().setUniform2f("u_xySeed", randParams.xySeed);
    //
    m_computeShader.compute(particleSystem.getNbParticles());
    m_computeShader.get().unbind();
}