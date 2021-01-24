#pragma once

class StateModifier;

struct RandomParams {
	RandomParams();
	float seed;
	glm::vec2 xySeed;

	void ImGui(StateModifier& stateModifier);

	void setApplyAndRecord_Seed(float seed, StateModifier& stateModifier);
	void setApplyAndRecord_SeedXY(const glm::vec2& seedXY, StateModifier& stateModifier);

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(seed),
			CEREAL_NVP(xySeed)
		);
	}
};