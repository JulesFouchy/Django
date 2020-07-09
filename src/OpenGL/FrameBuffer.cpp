#include "FrameBuffer.h"

#include "Helper/DisplayInfos.h"

#include "Debugging/glException.h"

FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	m_texture.bind();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getID(), 0));
	m_texture.unbind();
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::~FrameBuffer() {
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void FrameBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	GLCall(glGetIntegerv(GL_VIEWPORT, m_prevViewportSettings)); // Store viewport settings to restore them when unbinding
	GLCall(glViewport(0, 0, m_texture.getWidth(), m_texture.getHeight()));
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(m_prevViewportSettings[0], m_prevViewportSettings[1], m_prevViewportSettings[2], m_prevViewportSettings[3]));
}

void FrameBuffer::blitToScreen() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId);
	glBlitFramebuffer(0, 0, m_texture.getWidth(), m_texture.getHeight(), 0, 0, DisplayInfos::Width(), DisplayInfos::Height(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
}