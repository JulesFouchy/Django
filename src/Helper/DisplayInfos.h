#pragma once

class DisplayInfos {
public:
	static void RefreshSize(SDL_Window* window);

	static inline unsigned int Width()  { return m_windowWidth; }
	static inline unsigned int Height() { return m_windowHeight; }
	static inline float ScreenAspectRatio() { return m_screenAspectRatio; }
	static inline float RenderTargetAspectRatio() { return m_renderTargetAspectRatio; }

	static inline void SetRenderTargetAspectRatio(float ratio) { m_renderTargetAspectRatio = ratio; }

private:
	static unsigned int m_windowWidth;
	static unsigned int m_windowHeight;
	static float m_screenAspectRatio;
	static float m_renderTargetAspectRatio;
};