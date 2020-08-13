#pragma once

#include "Actions/ActionRef.h"

struct ActionTimestamp {
	ActionRef action;
	float time;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(action),
			CEREAL_NVP(time)
		);
	}
};

class ActionRecorder {
public:
	ActionRecorder();
	~ActionRecorder();

	void record(const ActionRef& actionRef);

private:
	float m_startTime;
	ActionRef m_startAction;
	std::vector<ActionTimestamp> m_timeline;
};