#include "RecState_Idle.h"

#include "RecState_Recording.h"
#include "RecState_Playing.h"

#include "RecordManager.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"

void RecState_Idle::update(ConfigManager& configManager, ParticlesSystem& partSystem) {
	// R.updateClock();
}

void RecState_Idle::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	bool bStateChanged = false;
	// Start recording
	if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1))) {
		R.setState<RecState_Recording>(configManager.getCurrentConfigRef());
		bStateChanged = true;
	}
	if (!bStateChanged) {
		ImGui::SameLine();
		// Start playing
		if (R.hasARecordSelected()) {
			if (MyImGui::ButtonWithIcon(Textures::Play())) {
				R.setState<RecState_Playing>(R.selectedRecord(), configManager, partSystem);
				bStateChanged = true;
			}
		}
	}
	if (!bStateChanged) {
		// Timeline
		if (R.hasARecordSelected()) {
			float t = 0.0f;
			const float duration = R.selectedRecord().totalDuration();
			if (MyImGui::Timeline("", &t, duration)) {
				R.setState<RecState_Playing>(R.selectedRecord(), configManager, partSystem);
			}
		}
	}
}