#include "RandomParams.h"

#include "Helper/Random.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"

RandomParams::RandomParams() 
	: seed(10.0f * MyRand::_m1to1()),
	  xySeed(glm::vec2(
		   10.0f * MyRand::_m1to1(),
		   10.0f * MyRand::_m1to1()
	  ))
{}

void RandomParams::ImGui(StateModifier& stateModifier) {
    if (ImGui::DragFloat("Seed", &seed)) {
        stateModifier.recordChange({ StateChangeType::Random_Seed, seed });
        stateModifier.apply();
    }
    if (ImGui::DragFloat2("X/Y Seed", &xySeed[0])) {
        stateModifier.recordChange({ StateChangeType::Random_XYSeed, xySeed });
        stateModifier.apply();
    }
}

void RandomParams::setApplyAndRecord_Seed(float seed, StateModifier& stateModifier) {
    this->seed = seed;
    stateModifier.recordChange({ StateChangeType::Random_Seed, seed });
    stateModifier.apply();
}

void RandomParams::setApplyAndRecord_SeedXY(const glm::vec2& seedXY, StateModifier& stateModifier) {
    this->xySeed = seedXY;
    stateModifier.recordChange({ StateChangeType::Random_XYSeed, xySeed });
    stateModifier.apply();
}