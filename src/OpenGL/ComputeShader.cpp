#include "ComputeShader.h"

#define WORK_GROUP_SIZE 256

ComputeShader::ComputeShader(const std::string& filepath)
{
    m_shader.addShader(ShaderType::Compute, filepath);
    m_shader.createProgram();
}

void ComputeShader::compute(unsigned int nbComputations) {
    m_shader.bind();
    glDispatchCompute(nbComputations / WORK_GROUP_SIZE + 1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    m_shader.unbind();
}