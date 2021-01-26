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
    if (LiveMode::ShowHelpMarkers()) {
        ImGui::SameLine();
        MyImGui::HelpMarker(R"V0G0N(Changing this number changes the random values used in some configurations (Like the "Random" layout).
You can also achieve the same effect by pressing Space.
)V0G0N");
    }
    if (ImGui::DragFloat2("X/Y Seed", &xySeed[0])) {
        stateModifier.recordChange({ StateChangeType::Random_XYSeed, xySeed });
        stateModifier.apply();
    }
    if (LiveMode::ShowHelpMarkers()) {
        ImGui::SameLine();
        MyImGui::HelpMarker(R"V0G0N(Changing this number changes the random values used in some configurations, but only along one axis.
)V0G0N");
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