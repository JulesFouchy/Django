#pragma once

#include "Presets.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/QuadVAO.h"
#include "OpenGL/RenderBuffer.h"

struct AlphaTrailSettingsValues {
	bool bAlphaTrail = true;
	float alphaTrailDecay = 20.0f;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bAlphaTrail),
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

	inline RenderBuffer& getRenderBuffer() { return m_renderBuffer; }

private:
	AlphaTrailSettingsValues m_values;
	Presets<AlphaTrailSettingsValues> m_presets;

	ShaderPipeline m_clearScreenPipeline;
	QuadVAO m_fullScreenVAO;
	RenderBuffer m_renderBuffer;

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