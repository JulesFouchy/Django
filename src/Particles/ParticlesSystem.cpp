#include "ParticlesSystem.h"

#include "Helper/DisplayInfos.h"
#include "Helper/Random.h"

#define ACTUAL_POS_ID 0
#define REST_POS_ID 1

ParticlesSystem::ParticlesSystem()
    : m_particlesSSBO(0, 4, GL_DYNAMIC_DRAW), // TODO check which hint is best
      m_restPositionsSSBO(1, 2, GL_STATIC_DRAW),
      m_colorsSSBO(2, 3, GL_STATIC_DRAW),
      m_physicsShader("res/shaders/physics.comp")
{
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

void ParticlesSystem::recomputeVBO(float partRadiusRelToHeight) {
    float radius = partRadiusRelToHeight;
    float halfW = radius;
    float halfH = radius;
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
    GLCall(glDeleteBuffers(1, &m_vboID));
    GLCall(glDeleteVertexArrays(1, &m_vaoID));
}

void ParticlesSystem::draw() {
    GLCall(glBindVertexArray(m_vaoID));
    //GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particlesSSBOid));
    GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_nbParticles));
}

void ParticlesSystem::updatePositions() {
    m_physicsShader.compute(m_nbParticles);
}

void ParticlesSystem::sendRestPositionsToGPU() {
    m_restPositionsSSBO.uploadData(m_nbParticles, (float*)m_restPositions.data());
}

void ParticlesSystem::setNbParticles(unsigned int newNbParticles) {
    // Set
    m_nbParticles = newNbParticles;
    // Rest positions
    m_restPositions.resize(m_nbParticles);
    // SSBOs
    m_particlesSSBO.uploadData(m_nbParticles, nullptr);
    m_restPositionsSSBO.uploadData(m_nbParticles, nullptr);
    // Colors
    std::vector<color::rgb<float>> colors;
    colors.resize(m_nbParticles);
    for (int i = 0; i < m_nbParticles; ++i) {
        float t = i / (float)m_nbParticles;
        colors[i] = color::hsv<float>({ t * 360.0f, 80.0f, 80.0f });
        //spdlog::info("{} {} {}", ::color::get::red(colors[i]), ::color::get::green(colors[i]), ::color::get::blue(colors[i]));
    }
    m_colorsSSBO.uploadData(m_nbParticles, (float*)colors.data());
    // Update physics shader
    m_physicsShader.get().bind();
    m_physicsShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
    m_physicsShader.get().unbind();
}