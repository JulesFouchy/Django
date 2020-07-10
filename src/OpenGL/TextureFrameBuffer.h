#pragma once

#include "FrameBuffer.h"

class TextureFrameBuffer : public FrameBuffer {
public:
	TextureFrameBuffer() = default;
	~TextureFrameBuffer();

private:
	void createAttachments(unsigned int width, unsigned int height) override;
	void destroyAttachments() override;
	virtual void attachAttachments() override;

private:
	unsigned int m_colorTextureId;
	unsigned int m_depthRenderBufferId;
};