#pragma once

#include "Actions/ActionRef.h"
#include <variant>

enum class EventType {
	Action,
	Param_Wheel
};

using EventValue = std::variant<ActionRef, float>;

class Event {
public:
	Event() = default;
	Event(const ActionRef& actionRef)
		: m_type(EventType::Action)
	{
		m_value = actionRef;
	}

	inline ActionRef& getActionRef() { // TODO remove me
		return std::get<ActionRef>(m_value);
	}

private:
	EventType m_type;
	EventValue m_value;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_type)
		);
	}
};