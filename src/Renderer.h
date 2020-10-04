#pragma once

#include "OpenGL/QuadVAO.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/TextureFrameBuffer.h"
#include "OpenGL/ShaderPipeline.h"

struct AlphaTrailSettingsValues;

class Renderer {
public:
	Renderer();
	~Renderer() = default;

	void onRenderBegin(float dt, const glm::vec3& bgColor, const AlphaTrailSettingsValues& alphaTrail);

	void onRenderSizeChanged(const glm::vec3& bgColor);

	void clearRenderBuffer(const glm::vec3& clearColor);
	void drawFullScreen();
	void drawFullScreenWithUVs();

	inline RenderBuffer& renderBuffer() { return m_renderBuffer; }

private:
	RenderBuffer m_renderBuffer; // we render here, and then blit it to screen (prevents visual artifacts when applying alpha trail)
	TextureFrameBuffer m_prevFrameTexture; // used to sample the previous frame (used to fix residuals of alpha trail)

	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;

	ShaderPipeline m_clearScreenPipeline;
	ShaderPipeline m_clearScreenNoResidualsPipeline;
};