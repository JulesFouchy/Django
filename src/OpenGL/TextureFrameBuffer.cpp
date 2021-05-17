#include "TextureFrameBuffer.h"

TextureFrameBuffer::~TextureFrameBuffer() {
	destroyAttachments();
}

void TextureFrameBuffer::createAttachments(int width, int height) {
	GLCall(glGenTextures(1, &m_colorTextureId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void TextureFrameBuffer::destroyAttachments() {
	GLCall(glDeleteTextures(1, &m_colorTextureId));
}

void TextureFrameBuffer::attachAttachments() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, getFrameBufferId()));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureId, 0));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void TextureFrameBuffer::attachTextureToSlot(unsigned int slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
}