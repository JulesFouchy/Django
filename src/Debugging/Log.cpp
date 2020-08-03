#include "Log.h"

void Log::Initialize() {
	spdlog::set_pattern("%^[%e] %n: %v%$");
}

void Log::separationLine() {
	spdlog::info("---------------------------------------------------------------");
}
void Log::log(glm::vec2 v) {
	spdlog::info("{} {}", v.x, v.y);
}
void Log::log(glm::vec3 v){
	spdlog::info("{} {} {}", v.x, v.y, v.z);
}