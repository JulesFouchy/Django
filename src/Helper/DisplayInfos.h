#pragma once

class DisplayInfos {
public:
	static void RefreshSize(SDL_Window* window);

	static inline unsigned int Width()  { return m_windowWidth; }
	static inline unsigned int Height() { return m_windowHeight; }
	static inline float Ratio() { return m_aspectRatio; }

private:
	static unsigned int m_windowWidth;
	static unsigned int m_windowHeight;
	static float m_aspectRatio;
};