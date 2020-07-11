#include "ParticlesSystem.h"

#include "Helper/DisplayInfos.h"
#include "Helper/Random.h"

#include "Settings/ColorSettings.h"

#define ACTUAL_POS_ID 0
#define REST_POS_ID 1

ParticlesSystem::ParticlesSystem()
    : m_particlesSSBO(0, 4, GL_DYNAMIC_DRAW), // TODO check which hint is best
      m_restPositionsSSBO(1, 2, GL_STATIC_DRAW),
      m_colorsSSBO(2, 3, GL_STATIC_DRAW),
      m_physicsShader("res/shaders/physics.comp"),
      m_colorGradientComputeShader("res/shaders/colorGradient.comp"),
      m_hueGradientComputeShader("res/shaders/hueGradient.comp")
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

void ParticlesSystem::setNbParticles(unsigned int newNbParticles, const ColorSettingsValues& colorSettings) {
    // Set
    m_nbParticles = newNbParticles;
    // Rest positions
    m_restPositions.resize(m_nbParticles);
    // Resize SSBOs
    m_particlesSSBO.uploadData(m_nbParticles, nullptr);
    m_restPositionsSSBO.uploadData(m_nbParticles, nullptr);
    m_colorsSSBO.uploadData(m_nbParticles, nullptr);
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
    setParticlesColors(colorSettings);
}

void ParticlesSystem::setParticlesColors(const ColorSettingsValues& colorSettings) {
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