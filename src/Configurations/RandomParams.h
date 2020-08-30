#pragma once

class StateModifier;

struct RandomParams {
	RandomParams();
	float seed;
	glm::vec2 xySeed;

	void ImGui(StateModifier& stateModifier);

	void setApplyAndRecord_Seed(float seed, StateModifier& stateModifier);
	void setApplyAndRecord_SeedXY(const glm::vec2& seedXY, StateModifier& stateModifier);
};