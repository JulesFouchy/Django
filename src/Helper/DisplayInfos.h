#pragma once

struct SDL_Window;

class DisplayInfos {
public:
	static void RefreshSize(SDL_Window* window);

	static inline int Width()  { return m_windowWidth; }
	static inline int Height() { return m_windowHeight; }
	static inline float Ratio() { return m_aspectRatio; }

private:
	static int m_windowWidth;
	static int m_windowHeight;
	static float m_aspectRatio;
};