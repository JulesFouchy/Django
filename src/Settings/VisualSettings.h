#pragma once

class ParticlesSystem;

class VisualSettings {
public:
	VisualSettings(bool isAlphaTrailEnabled, float alphaTrailDecay, const glm::vec3& backgroundColor);
	~VisualSettings() = default;
	static const VisualSettings DEFAULT;

	void apply();
	void ImGui();

	static void  EnableAlphaTrail();
	static void DisableAlphaTrail();
	inline bool isAlphaTrailEnabled() { return m_bAlphaTrail; }
	inline float alphaTrailDecay() { return m_alphaTrailDecay; }
	inline const glm::vec3& backgroundColor() { return m_bgColor; }

private:
	bool m_bAlphaTrail;
	float m_alphaTrailDecay;
	glm::vec3 m_bgColor;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_bAlphaTrail),
			CEREAL_NVP(m_alphaTrailDecay),
			CEREAL_NVP(m_bgColor.r),
			CEREAL_NVP(m_bgColor.g),
			CEREAL_NVP(m_bgColor.b)
		);
	}
};