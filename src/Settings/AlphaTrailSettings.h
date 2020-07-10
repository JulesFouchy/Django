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

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bAlphaTrail),
			CEREAL_NVP(bFixResiduals),
			CEREAL_NVP(alphaTrailDecay)
		);
	}
};

class AlphaTrailSettings {
public:
	AlphaTrailSettings();
	~AlphaTrailSettings() = default;

	void apply(const glm::vec3& bgColor);
	void ImGui(const glm::vec3& bgColor);

	void clearScreen(float dt, const glm::vec3& bgColor);
	void finishRendering();

	inline void setSize(unsigned int width, unsigned int height) { m_renderBuffer.setSize(width, height); m_textureFrameBuffer.setSize(width, height); }

private:
	AlphaTrailSettingsValues m_values;
	Presets<AlphaTrailSettingsValues> m_presets;

	ShaderPipeline m_clearScreenPipeline;
	ShaderPipeline m_clearScreenNoResidualsPipeline;
	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;
	RenderBuffer m_renderBuffer;
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