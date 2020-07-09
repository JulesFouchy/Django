#pragma once

class RenderBuffer {
public:
	RenderBuffer();
	~RenderBuffer();

	void setSize(unsigned int width, unsigned int height);

	void bind();
	void unbind();
	void blitToScreen();

private:
	void createAttachments(unsigned int width, unsigned int height);
	void destroyAttachments();

private:
	unsigned int m_colorRenderBufferId;
	unsigned int m_depthRenderBufferId;
	unsigned int m_frameBufferId;
	int m_prevViewportSettings[4];
	unsigned int m_width;
	unsigned int m_height;
};