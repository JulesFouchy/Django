#pragma once

#include "ActionRef.h"

struct Action {
	ActionRef ref;
	unsigned int thumbnailTextureID = -1;
	size_t index;

	Action(const std::string& name, unsigned int thumbnailTextureID, ActionType type, size_t index = -1)
		: ref(name, type), thumbnailTextureID(thumbnailTextureID), index(index)
	{}
	Action() = default;
	~Action() = default;
};