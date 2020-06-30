#pragma once

class VisualSettings {
public:
	VisualSettings(
		bool             isAlphaTrailEnabled = true,
		float            alphaTrailDecay     = 20.0f,
		const glm::vec3& backgroundColor     = glm::vec3(0.0f, 0.0f, 0.0f)
	);
	~VisualSettings() = default;

	void apply();
	void ImGui();

	// Getters
	inline bool isAlphaTrailEnabled() { return m_bAlphaTrail; }
	inline float alphaTrailDecay() { return m_alphaTrailDecay; }
	inline const glm::vec3& backgroundColor() { return m_bgColor; }

private:
	static void  EnableAlphaTrail();
	static void DisableAlphaTrail();

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