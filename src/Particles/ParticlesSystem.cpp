#include "ParticlesSystem.h"

#include <glad/glad.h>

#include "Debugging/glException.h"

#include "Helper/DisplayInfos.h"
#include "Helper/Random.h"

#include <imgui/imgui.h>

#define WORK_GROUP_SIZE 256
#define ACTUAL_POS_ID 0
#define REST_POS_ID 1

ShaderPipeline ParticlesSystem::m_physicsShader;

void ParticlesSystem::Initialize() {
    // Compute shader
    m_physicsShader.addShader(ShaderType::Compute, "res/shaders/physics.comp");
    m_physicsShader.createProgram();
}

ParticlesSystem::ParticlesSystem(unsigned int nbParticles)
    : m_nbParticles(nbParticles),
      m_particlesSSBO(0, 4, GL_DYNAMIC_DRAW), // TODO check which hint is best
      m_restPositionsSSBO(1, 2, GL_STATIC_DRAW),
      m_colorsSSBO(2, 3, GL_STATIC_DRAW),
      m_physicsSettings()
{
    m_restPositions.resize(nbParticles);
    // Uniforms
    m_physicsSettings.setUniforms();
    // SSBOs
    m_particlesSSBO.uploadData(nbParticles, nullptr);
    m_restPositionsSSBO.uploadData(nbParticles, nullptr);
        // Colors
    std::vector<glm::vec3> colors;
    colors.resize(nbParticles);
    for (int i = 0; i < m_nbParticles; ++i) {
        float t = i / (float)m_nbParticles;
        colors[i] = t * glm::vec3(1, 0, 0) + (1-t) * glm::vec3(0,0,1);
        //colors[i] = glm::vec3(1, 0,0);
    }
    m_colorsSSBO.uploadData(nbParticles, (float*)colors.data());
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
    float radius = 0.0135f;
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
    GLCall(glDeleteBuffers(1, &m_vboID));
    GLCall(glDeleteVertexArrays(1, &m_vaoID));
}

void ParticlesSystem::draw() {
    GLCall(glBindVertexArray(m_vaoID));
    //GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particlesSSBOid));
    GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_nbParticles));
}

void ParticlesSystem::updatePositions() {
    m_physicsShader.bind();
    glDispatchCompute(m_nbParticles / WORK_GROUP_SIZE + 1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    m_physicsShader.unbind();
}

void ParticlesSystem::sendRestPositionsToGPU() {
    m_restPositionsSSBO.uploadData(m_nbParticles, (float*)m_restPositions.data());
}

void ParticlesSystem::ImGui_Windows() {
    ImGui::Begin("Physics parameters");
    m_physicsSettings.ImGui_Parameters();
    ImGui::End();
}