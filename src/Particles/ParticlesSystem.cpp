#include "ParticlesSystem.h"

#include "Helper/DisplayInfos.h"
#include "Helper/Random.h"

#include "Settings/ColorSettings.h"

#define ACTUAL_POS_ID 0
#define REST_POS_ID 1

ParticleSystem::ParticleSystem()
    : m_particlesSSBO(0, GL_DYNAMIC_DRAW), // TODO check which hint is best
      m_restPositionsSSBO(1, GL_STATIC_DRAW),
      m_colorsSSBO(2, GL_STATIC_DRAW),
      m_physicsShader("internal-shaders/physics.comp"),
      m_colorGradientComputeShader("internal-shaders/colorGradient.comp"),
      m_hueGradientComputeShader("internal-shaders/hueGradient.comp")
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

void ParticleSystem::recomputeVBO(float partRadiusRelToHeight) {
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

ParticleSystem::~ParticleSystem() {
    GLCall(glDeleteBuffers(1, &m_vboID));
    GLCall(glDeleteVertexArrays(1, &m_vaoID));
}

void ParticleSystem::draw() {
    GLCall(glBindVertexArray(m_vaoID));
    GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_nbParticles));
}

void ParticleSystem::updatePositions() {
    m_physicsShader.compute(m_nbParticles);
}

void ParticleSystem::sendRestPositionsToGPU() {
    m_restPositionsSSBO.uploadData(m_nbParticles * 2, (float*)m_restPositions.data());
}

void ParticleSystem::applyNbParticles(unsigned int newNbParticles, const ColorSettingsValues& colorSettings) {
    // Set
    m_nbParticles = newNbParticles;
    // Rest positions
    m_restPositions.resize(m_nbParticles);
    // Resize SSBOs
    m_particlesSSBO.uploadData(m_nbParticles * 4, nullptr);
    m_restPositionsSSBO.uploadData(m_nbParticles * 2, nullptr);
    m_colorsSSBO.uploadData(m_nbParticles * 3, nullptr);
    // Update uniform
    m_physicsShader.get().bind();
    m_physicsShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
    m_physicsShader.get().unbind();
    m_colorGradientComputeShader.get().bind();
    m_colorGradientComputeShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
    m_colorGradientComputeShader.get().unbind();
    m_hueGradientComputeShader.get().bind();
    m_hueGradientComputeShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
    m_hueGradientComputeShader.get().unbind();
    // Colors
    applyParticleColors(colorSettings);
}

void ParticleSystem::applyParticleColors(const ColorSettingsValues& colorSettings) {
    // Hue gradient mode
    if (colorSettings.bColorModeHueGradient) {
        m_hueGradientComputeShader.get().bind();
        m_hueGradientComputeShader.get().setUniform1f("hueStart", colorSettings.particlesHueStart / 360.0f);
        m_hueGradientComputeShader.get().setUniform1f("hueEnd",   colorSettings.particlesHueEnd   / 360.0f);
        m_hueGradientComputeShader.get().setUniform1f("saturation", colorSettings.particleSaturation / 100.0f);
        m_hueGradientComputeShader.get().setUniform1f("value", colorSettings.particleValue / 100.0f);
        m_hueGradientComputeShader.compute(m_nbParticles);
    }
    // Color gradient mode
    else {
        m_colorGradientComputeShader.get().bind();
        m_colorGradientComputeShader.get().setUniform3f("colorStart", colorSettings.particleColorStart);
        m_colorGradientComputeShader.get().setUniform3f("colorEnd",   colorSettings.particleColorEnd);
        m_colorGradientComputeShader.compute(m_nbParticles);
    }
}