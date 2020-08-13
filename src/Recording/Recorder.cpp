#include "Recorder.h"

#include "Actions/ActionRef.h"
#include "Time/Time_Realtime.h"

Recorder::Recorder()
	: m_time(std::make_unique<Time_Realtime>())
{}

void Recorder::onAction(const ActionRef& actionRef) {
	if (m_bRecording) {
		m_actionsRecording.push(actionRef, m_time->time());
	}
}

void Recorder::update() {
	m_time->update();
}