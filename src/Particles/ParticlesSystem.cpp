#include "ParticlesSystem.h"

#include <glad/glad.h>

#include "Debugging/glException.h"

#include "Helper/DisplayInfos.h"
#include "Helper/Random.h"

#define WORK_GROUP_SIZE 256
#define ACTUAL_POS_ID 0
#define REST_POS_ID 1

ParticlesSystem::ParticlesSystem(unsigned int nbParticles)
    : m_nbParticles(nbParticles)
{
    // Compute shader
    m_computeShader.addShader(ShaderType::Compute, "res/shaders/updatePositions.comp");
    m_computeShader.createProgram();
    //
    for (size_t i = 0; i < nbParticles; ++i)
        m_restPositions.emplace_back(MyRand::_m1to1(), MyRand::_m1to1());
    // Actual positions buffer
    GLCall(glGenBuffers(1, &m_actualPosSSBOid));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_actualPosSSBOid));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbParticles * 2 * sizeof(float), m_restPositions.data(), GL_DYNAMIC_DRAW)); // TODO check which hint is best
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ACTUAL_POS_ID, m_actualPosSSBOid));
    // Rest positions buffer
    GLCall(glGenBuffers(1, &m_restPosSSBOid));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_restPosSSBOid));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbParticles * 2 * sizeof(float), m_restPositions.data(), GL_STATIC_DRAW));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, REST_POS_ID, m_restPosSSBOid));
    // Vertex buffer
    GLCall(glGenBuffers(1, &m_vboID));
    // Vertex array
    GLCall(glGenVertexArrays(1, &m_vaoID));
    GLCall(glBindVertexArray(m_vaoID));
        // Quad positions
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
        // Quad tex coords
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void ParticlesSystem::recomputeVBO() {
    float radius = 0.015f;
    float halfW = radius;
    float halfH = radius * DisplayInfos::Ratio();
    float vertices[] = {
        -halfW, -halfH,  0.0f, 0.0f,
         halfW, -halfH,  1.0f, 0.0f,
         halfW,  halfH,  1.0f, 1.0f,

        -halfW, -halfH,  0.0f, 0.0f,
         halfW,  halfH,  1.0f, 1.0f,
        -halfW,  halfH,  0.0f, 1.0f
    };
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

ParticlesSystem::~ParticlesSystem() {
    GLCall(glDeleteBuffers(1, &m_restPosSSBOid));
    GLCall(glDeleteBuffers(1, &m_actualPosSSBOid));
}

void ParticlesSystem::draw() {
    GLCall(glBindVertexArray(m_vaoID));
    //GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_actualPosSSBOid));
    GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_nbParticles));
}

void ParticlesSystem::updatePositions() {
    m_computeShader.bind();
    glDispatchCompute(m_nbParticles / WORK_GROUP_SIZE + 1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    m_computeShader.unbind();
}

void ParticlesSystem::sendRestPositionsToGPU() {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_restPosSSBOid));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, m_nbParticles * 2 * sizeof(float), m_restPositions.data(), GL_STATIC_DRAW));
}