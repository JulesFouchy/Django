#include "TextureFrameBuffer.h"

TextureFrameBuffer::~TextureFrameBuffer() {
	destroyAttachments();
}

void TextureFrameBuffer::createAttachments(unsigned int width, unsigned int height) {
	// Color attachment
	GLCall(glGenTextures(1, &m_colorTextureId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	// Depth attachment
	GLCall(glGenRenderbuffers(1, &m_depthRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void TextureFrameBuffer::destroyAttachments() {
	GLCall(glDeleteTextures(1, &m_colorTextureId));
	GLCall(glDeleteRenderbuffers(1, &m_depthRenderBufferId));
}

void TextureFrameBuffer::attachAttachments() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, getFrameBufferId()));
	// Color attachment
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureId, 0));
	// Depth attachment
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId));
	//
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}