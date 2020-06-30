#include "SSBO.h"

SSBO::SSBO(unsigned int binding, int componentsCount, GLuint hint)
	: m_binding(binding), m_componentsCount(componentsCount), m_hint(hint)
{
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
}

SSBO::~SSBO() {
    GLCall(glDeleteBuffers(1, &m_id));
}

void SSBO::uploadData(int nbElements, float* data) {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbElements * m_componentsCount * sizeof(float), data, m_hint));
}