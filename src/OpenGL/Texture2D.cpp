#include "Texture2D.h"

#include "Debugging/glException.h"

Texture2D::Texture2D()
	: m_bytesPerPixel(3), m_GLpixelInternalFormat(GL_RGB8), m_GLpixelFormat(GL_RGB), m_GLpixelType(GL_UNSIGNED_BYTE)
{
	GLint interpolationMode = GL_NEAREST;
	GLint wrapMode = GL_CLAMP_TO_EDGE;

	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture2D::~Texture2D() {
	GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture2D::setSize(unsigned int width, unsigned int height) {
	m_width = width;
	m_height = height;
	m_aspectRatio = (float)m_width / m_height;

	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_GLpixelInternalFormat, m_width, m_height, 0, m_GLpixelFormat, m_GLpixelType, nullptr));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::bind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}