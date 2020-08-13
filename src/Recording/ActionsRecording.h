#pragma once

#include "Actions/ActionRef.h"

struct ActionTimestamp {
	ActionRef action;
	float time;

	ActionTimestamp::ActionTimestamp(const ActionRef& ref, float time)
		: action(ref), time(time)
	{}

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

class ActionsRecording {
public:
	ActionsRecording() = default;
	~ActionsRecording() = default;

	void start(const ActionRef& startAction, float time);
	void push (const ActionRef& actionRef,   float time);

private:
	ActionRef m_startAction;
	float m_startTime;
	std::vector<ActionTimestamp> m_timeline;

	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_startAction),
			CEREAL_NVP(m_startTime),
			CEREAL_NVP(m_timeline)
		);
	}
};