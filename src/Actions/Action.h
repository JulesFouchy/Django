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
	unsigned int thumbnailTextureID = -1;
	ActionType type;
	size_t index;

	Action(const std::string& name, unsigned int thumbnailTextureID, ActionType type, size_t index = -1)
		: name(name), thumbnailTextureID(thumbnailTextureID), type(type), index(index)
	{}
	Action() = default;
	~Action() = default;
};