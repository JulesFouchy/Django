#include "RecState_Recording.h"

#include "RecState_Idle.h"

#include "RecordManager.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Helper/File.h"

#include "Configurations/ConfigRef.h"

RecState_Recording::RecState_Recording(RecordManager& recManager, const ConfigRef& currentConfigRef)
	: RecState(recManager),
	m_startTime(R.m_clock->time()),
	m_record(R.m_records.emplace_back(currentConfigRef))
{}

RecState_Recording::~RecState_Recording() {
	// Serialize record
	if (!MyFile::Exists(R.folderPath()))
		std::filesystem::create_directory(R.folderPath());
	m_record.serialize(R.folderPath());
}

void RecState_Recording::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	// Stop recording
	if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1), ImVec4(1, 0.2, 0.2, 1))) {
		R.setState<RecState_Idle>();
	}
	ImGui::SameLine();
	// Play disabled
	MyImGui::ButtonWithIconDisabled(Textures::Play(), "Cannot play while recording a clip");
}

void RecState_Recording::onAction(const ActionRef& actionRef) {
	m_record.onAction(actionRef, timeSinceStart());
}

float RecState_Recording::timeSinceStart() {
	return R.m_clock->time() - m_startTime;
}