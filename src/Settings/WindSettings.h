#pragma once

class ShaderPipeline;

class WindSettings {
public:
	WindSettings() = default;
	~WindSettings() = default;

	void apply(ShaderPipeline& physicsCompute);
	void ImGui(ShaderPipeline& physicsCompute);
	void setWindOffset(ShaderPipeline& physicsCompute, float time);

private:
	void setNoiseFrequency(ShaderPipeline& physicsCompute);
	void setMaxStrength(ShaderPipeline& physicsCompute);
	void setMinStrength(ShaderPipeline& physicsCompute);
	void setDirection(ShaderPipeline& physicsCompute);

private:
	float m_noiseFrequency = 0.319f;
	float m_minStrength = -0.048f;
	float m_maxStrength = 0.968f;
	float m_speed = 0.304f;
	float m_directionAngle = 2.849f;
	glm::vec2 m_direction = glm::vec2(-0.956f, 0.292f);

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_noiseFrequency),
			CEREAL_NVP(m_minStrength),
			CEREAL_NVP(m_maxStrength),
			CEREAL_NVP(m_speed),
			CEREAL_NVP(m_directionAngle),
			CEREAL_NVP(m_direction.x),
			CEREAL_NVP(m_direction.y)
		);
	}
};