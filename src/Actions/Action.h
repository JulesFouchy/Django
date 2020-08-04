#pragma once

enum class ActionType {
	SHAPE,
	SVG_SHAPE,
	LAYOUT,
	STANDALONE,
	TEXT,
	REROLL_RANDOM
};

struct Action {
	std::string name;
	ActionType type;
	size_t index;

	Action(const std::string& name, ActionType type, size_t index = -1)
		: name(name), type(type), index(index)
	{}
	Action() = default;
	~Action() = default;
};