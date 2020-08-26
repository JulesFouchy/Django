#pragma once

#include "Presets.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/QuadVAO.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/TextureFrameBuffer.h"

struct AlphaTrailSettingsValues {
	bool bAlphaTrail = true;
	bool bFixResiduals = false;
	float alphaTrailDecay = 20.0f;
	float threshold = 0.233;
	float minAlpha = 0.112;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bAlphaTrail),
			CEREAL_NVP(bFixResiduals),
			CEREAL_NVP(alphaTrailDecay),
			CEREAL_NVP(threshold),
			CEREAL_NVP(minAlpha)
		);
	}
};

class AlphaTrailSettings {
public:
	AlphaTrailSettings();
	~AlphaTrailSettings() = default;

	void apply();
	void ImGui(const glm::vec3& bgColor);

	void clearScreen(float dt, const glm::vec3& bgColor);
	void finishRendering();

	void setSize(unsigned int width, unsigned int height);
	void attachOffscreenRenderbuffer(RenderBuffer& renderBuffer);
	void detachOffscreenRenderBuffer();

private:
	void clearRenderBuffer(const glm::vec3& bgColor);
	RenderBuffer& renderBuffer();

private:
	AlphaTrailSettingsValues m_values;
	Presets<AlphaTrailSettingsValues> m_presets;

	ShaderPipeline m_clearScreenPipeline;
	ShaderPipeline m_clearScreenNoResidualsPipeline;
	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;
	RenderBuffer m_renderBuffer; // used to then blit on screen (prevents visual artifacts when applying alpha trail)
	RenderBuffer* m_targetRenderBuffer; // used if we want to render on this render buffer instead of screen e.g. when exporting high res images
	TextureFrameBuffer m_textureFrameBuffer;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_values),
			CEREAL_NVP(m_presets)
		);
	}
};