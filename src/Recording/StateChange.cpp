#include "StateChange.h"

StateChange::StateChange(StateChangeType type, const StateChangeValue& value)
	: m_type(type), m_value(value)
{}