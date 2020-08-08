#include "DrawOnTextureFramebuffer.h"

#include "Helper/DisplayInfos.h"

#include "Debugging/glException.h"

DrawOnTextureFramebuffer::DrawOnTextureFramebuffer() {
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
}

DrawOnTextureFramebuffer::~DrawOnTextureFramebuffer() {
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void DrawOnTextureFramebuffer::setRenderTarget_Texture(unsigned int textureID, unsigned int width, unsigned int height) {
	// Bind Framebuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	// Set viewport
	GLCall(glViewport(0, 0, width, height));
	// Bind and Attach texture
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0));
}

void DrawOnTextureFramebuffer::setRenderTarget_Screen(){
	// Unbind FrameBuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	// Set viewport
	glViewport(0, 0, DisplayInfos::Width(), DisplayInfos::Height());
}