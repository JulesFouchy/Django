#pragma once

class DrawOnTextureFramebuffer {
public:
	DrawOnTextureFramebuffer();
	~DrawOnTextureFramebuffer();

	void setRenderTarget_Texture(unsigned int textureID, int width, int height);
	void setRenderTarget_Screen();

protected:
	unsigned int m_frameBufferId;
};