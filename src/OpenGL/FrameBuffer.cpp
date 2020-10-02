#include "FrameBuffer.h"

#include "Helper/DisplayInfos.h"

#include "Debugging/glException.h"

FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
}

FrameBuffer::~FrameBuffer() {
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void FrameBuffer::setSize(unsigned int width, unsigned int height) {
	m_width = width;
	m_height = height;
	destroyAttachments();
	createAttachments(width, height);
	attachAttachments();
}

void FrameBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	GLCall(glGetIntegerv(GL_VIEWPORT, m_prevViewportSettings)); // Store viewport settings to restore them when unbinding
	GLCall(glViewport(0, 0, m_width, m_height));                // Only usefull if we plan on using this frame buffer at a different resolution than the screen's
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(m_prevViewportSettings[0], m_prevViewportSettings[1], m_prevViewportSettings[2], m_prevViewportSettings[3]));
}

void FrameBuffer::blitToScreen(const glm::vec2& corner1, const glm::vec2& corner2) {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId));
	GLCall(glBlitFramebuffer(0, 0, m_width, m_height, corner1.x, corner1.y, corner2.x, corner2.y, GL_COLOR_BUFFER_BIT, GL_LINEAR));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::blitToScreenWithCareToAspectRatio(const glm::vec2& pos, const glm::vec2& size) {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId));
	// Gray background
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Fit frame buffer as well as possible
	float ratio = aspectRatio();
	if (ratio < DisplayInfos::ScreenAspectRatio()) {
		// Fit height
		float halfW = DisplayInfos::Height() * ratio * 0.5f;
		float c = DisplayInfos::Width() * 0.5f;
		GLCall(glBlitFramebuffer(0, 0, m_width, m_height, static_cast<int>(c - halfW), 0, static_cast<int>(c + halfW), DisplayInfos::Height(), GL_COLOR_BUFFER_BIT, GL_LINEAR));
	}
	else {
		// Fit width
		float halfH = DisplayInfos::Width() / ratio * 0.5f;
		float c = DisplayInfos::Height() * 0.5f;
		// TODO : 
		GLCall(glBlitFramebuffer(0, 0, m_width, m_height, 0, static_cast<int>(c - halfH), DisplayInfos::Width(), static_cast<int>(c + halfH), GL_COLOR_BUFFER_BIT, GL_LINEAR));
	}
	//
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::blitTo(FrameBuffer& frameBuffer) {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer.getFrameBufferId()));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId));
	GLCall(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, frameBuffer.m_width, frameBuffer.m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}