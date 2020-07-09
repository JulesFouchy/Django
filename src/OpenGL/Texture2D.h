#pragma once

class Texture2D {
public:
	Texture2D();
	~Texture2D();

	void setSize(unsigned int width, unsigned int height);

	void bind();
	void unbind();

	inline unsigned int getID() { return m_textureID; }
	inline unsigned int getWidth() { return m_width; }
	inline unsigned int getHeight() { return m_height; }

private:
	unsigned int m_width;
	unsigned int m_height;
	float m_aspectRatio;

	unsigned int m_bytesPerPixel;
	GLint m_GLpixelInternalFormat;
	GLenum m_GLpixelFormat;
	GLenum m_GLpixelType;

	unsigned int m_textureID;
};