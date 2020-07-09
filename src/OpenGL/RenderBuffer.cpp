#include "RenderBuffer.h"

#include "Helper/DisplayInfos.h"

#include "Debugging/glException.h"

RenderBuffer::RenderBuffer()
{
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
}

RenderBuffer::~RenderBuffer() {
	destroyAttachments();
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void RenderBuffer::createAttachments(unsigned int width, unsigned int height) {
	// Color attachment
	GLCall(glGenRenderbuffers(1, &m_colorRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_colorRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height));
	// Depth attachment
	GLCall(glGenRenderbuffers(1, &m_depthRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
	//
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBuffer::destroyAttachments() {
	GLCall(glDeleteRenderbuffers(1, &m_colorRenderBufferId));
	GLCall(glDeleteRenderbuffers(1, &m_depthRenderBufferId));
}

void RenderBuffer::setSize(unsigned int width, unsigned int height) {
	m_width = width;
	m_height = height;
	destroyAttachments();
	createAttachments(width, height);
	// Attach attachments
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBufferId));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void RenderBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	GLCall(glGetIntegerv(GL_VIEWPORT, m_prevViewportSettings)); // Store viewport settings to restore them when unbinding
	GLCall(glViewport(0, 0, m_width, m_height));                // Only usefull if we plan on using this render buffer at a different resolution than the screen's
}

void RenderBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(m_prevViewportSettings[0], m_prevViewportSettings[1], m_prevViewportSettings[2], m_prevViewportSettings[3]));
}

void RenderBuffer::blitToScreen() {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId));
	GLCall(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, DisplayInfos::Width(), DisplayInfos::Height(), GL_COLOR_BUFFER_BIT, GL_LINEAR));
}