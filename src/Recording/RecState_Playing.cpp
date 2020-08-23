#include "RecState_Playing.h"

#include "RecState_Idle.h"

#include "RecordManager.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"

RecState_Playing::RecState_Playing(RecordManager& recManager, Record& record, ConfigManager& configManager, ParticlesSystem& partSystem)
	: RecState(recManager), m_record(record), m_startTime(R.m_clock->time())
{
	if (!m_record.startPlaying(configManager, partSystem))
		R.setState<RecState_Idle>();
}

void RecState_Playing::update(ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (!m_record.updatePlaying(timeSinceStart(), configManager, partSystem)
	&&  !m_bDraggingOnTheTimeline) // Prevent the playing from stopping just because we dragged the time cursor outside of the timeline
		R.setState<RecState_Idle>();
	//
	m_bDraggingOnTheTimeline = false; // Reset every frame. It is set by the ImGui() method before the call to update()
}

void RecState_Playing::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	bool bStateChanged = false;
	// Recording disabled
	MyImGui::ButtonWithIconDisabled(Textures::Record(), "Cannot record while playing a clip");
	ImGui::SameLine();
	// Stop playing
	if (MyImGui::ButtonWithIcon(Textures::Pause())) {
		R.setState<RecState_Idle>();
		bStateChanged = true;
	}
	if (!bStateChanged) {
		// Timeline
		float t = timeSinceStart();
		const float duration = m_record.totalDuration();
		if (MyImGui::Timeline("", &t, duration)) {
			R.m_clock->setTime(m_startTime + t);
			m_record.setTime(t, configManager, partSystem);
			m_bDraggingOnTheTimeline = true;
		}
		// Records list
		R.ImGuiRecordsList();
	}
}

float RecState_Playing::timeSinceStart() {
	return R.m_clock->time() - m_startTime;
}