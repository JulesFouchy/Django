#pragma once

#include "Presets.h"

class ShaderPipeline;
class StateModifier;

struct WindSettingsValues {
	float noiseFrequency = 0.319f;
	float minStrength = -0.048f;
	float maxStrength = 0.968f;
	float speed = 0.304f;
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(noiseFrequency),
			CEREAL_NVP(minStrength),
			CEREAL_NVP(maxStrength),
			CEREAL_NVP(speed)
		);
	}
};

struct WindDirectionSettingsValues {
	float directionAngle = 2.849f;
	glm::vec2 direction = glm::vec2(-0.956f, 0.292f); // must be == {cos(directionAngle), sin(directionAngle)} as defined above

	inline void computeDirection() {
		direction = glm::vec2(cos(directionAngle), sin(directionAngle));
	}

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(directionAngle)
		);
		computeDirection();
	}
};

class WindSettings {
public:
	WindSettings();
	~WindSettings() = default;

	void ImGui(StateModifier& stateModifier);

	void applyAndRecord(StateModifier& stateModifier);
	void applyAndRecord_Frequency(StateModifier& stateModifier);
	void applyAndRecord_MinStrength(StateModifier& stateModifier);
	void applyAndRecord_MaxStrength(StateModifier& stateModifier);
	void applyAndRecord_Speed(StateModifier& stateModifier);
	void applyAndRecord_Direction(StateModifier& stateModifier);

	inline void setFrequency(float frequency)     { m_values.noiseFrequency = frequency; m_presets.setToPlaceholderSetting(); }
	inline void setMinStrength(float minStrength) { m_values.minStrength = minStrength;  m_presets.setToPlaceholderSetting(); }
	inline void setMaxStrength(float maxStrength) { m_values.maxStrength = maxStrength;  m_presets.setToPlaceholderSetting(); }
	inline void setSpeed(float speed)             { m_values.speed = speed;              m_presets.setToPlaceholderSetting(); }
	inline void setDirection(float angle)         { m_dirValues.directionAngle = angle;  m_presets.setToPlaceholderSetting(); m_dirValues.computeDirection(); }

	void setWindOffsetInShader(ShaderPipeline& physicsCompute, float time);

	inline const WindSettingsValues& getValues() const { return m_values; }
	inline void setValues(const WindSettingsValues& values) { m_values = values; }
	inline float getDirAngle() const { return m_dirValues.directionAngle; }

private:
	void setFrequencyInShader(ShaderPipeline& physicsCompute);
	void setMinStrengthInShader(ShaderPipeline& physicsCompute);
	void setMaxStrengthInShader(ShaderPipeline& physicsCompute);
	void setDirectionInShader(ShaderPipeline& physicsCompute);

private:
	WindSettingsValues m_values;
	Presets<WindSettingsValues> m_presets;
	WindDirectionSettingsValues m_dirValues;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_values),
			CEREAL_NVP(m_presets),
			CEREAL_NVP(m_dirValues)
		);
	}
};