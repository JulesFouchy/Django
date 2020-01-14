#pragma once

#include "spdlog/spdlog.h"

#include "glm/glm.hpp"

namespace Log {
	void Initialize();

	void separationLine();
	void log(glm::vec2 v);
	void log(glm::vec3 v);
	void log(glm::vec4 v);
	void log(glm::mat3x3 mat);
	void log(glm::mat4x4 mat);
};