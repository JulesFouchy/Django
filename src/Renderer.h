#pragma once

#include "OpenGL/QuadVAO.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/TextureFrameBuffer.h"
#include "OpenGL/ShaderPipeline.h"

struct AlphaTrailSettingsValues;

class Renderer {
public:
	Renderer(std::function<void()> renderTargetChangeCallback);
	~Renderer() = default;

	void onRenderBegin(float dt, const glm::vec3& bgColor, const AlphaTrailSettingsValues& alphaTrail);
	void onRenderEnd(const AlphaTrailSettingsValues& alphaTrail);

	void onWindowResize(unsigned int width, unsigned int height);
	void attachRenderbuffer(RenderBuffer& renderBuffer, const glm::vec3& bgColor);
	void detachRenderBuffer();

	void clearRenderBuffer(const glm::vec3& clearColor);
	void drawFullScreen();
	void drawFullScreenWithUVs();

	//float aspectRatio() const;

private:
	RenderBuffer& renderBuffer();
	inline bool hasRenderBufferAttached() const { return (bool)m_targetRenderBuffer; }

private:
	RenderBuffer m_screenSizeRenderBuffer;        // used to then blit on screen (prevents visual artifacts when applying alpha trail)
	RenderBuffer* m_targetRenderBuffer = nullptr; // used if we want to render on this render buffer instead of screen e.g. when exporting high res images
	TextureFrameBuffer m_textureFrameBuffer;      // used to sample the previous frame (used to fix residuals of alpha trail)

	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;

	ShaderPipeline m_clearScreenPipeline;
	ShaderPipeline m_clearScreenNoResidualsPipeline;

	std::function<void()> m_renderTargetChangeCallback;
};