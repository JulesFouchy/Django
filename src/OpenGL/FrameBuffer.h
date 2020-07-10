#pragma once

class FrameBuffer {
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	virtual void setSize(unsigned int width, unsigned int height);

	void bind();
	void unbind();
	void blitToScreen();
	void blitTo(FrameBuffer& frameBuffer);

protected:
	inline unsigned int getFrameBufferId() { return m_frameBufferId; }

private:
	virtual void destroyAttachments() = 0;
	virtual void createAttachments(unsigned int width, unsigned int height) = 0;
	virtual void attachAttachments() = 0;

private:
	unsigned int m_frameBufferId;
	int m_prevViewportSettings[4];
	unsigned int m_width;
	unsigned int m_height;
};