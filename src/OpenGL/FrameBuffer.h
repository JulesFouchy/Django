#pragma once

#include "Texture2D.h"

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	void bind();
	void unbind();

	inline void setSize(unsigned int width, unsigned int height) { m_texture.setSize(width, height); }

protected:
	unsigned int m_frameBufferId;
	Texture2D m_texture;
	int m_prevViewportSettings[4];
};