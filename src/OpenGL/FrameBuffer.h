#pragma once

#include "Texture2D.h"

class FrameBuffer {
public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

	void attachTexture(Texture2D& texture);

	void bind();
	void bind_WithoutSettingViewport();
	void setViewport();
	void unbind();
	void unbind_WithoutRestoringViewport();
	void restoreViewport();

	void clear(); // Make sure you have bound the framebuffer beforehand

protected:
	unsigned int m_frameBufferId;
	Texture2D m_texture;
	int m_prevViewportSettings[4];
};