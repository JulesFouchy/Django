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
void Log::log(glm::vec4 v){
	spdlog::info("{} {} {} {}", v.x, v.y, v.z, v.w);
}
void Log::log(glm::mat3x3 mat){
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f}", mat[0][0], mat[1][0], mat[2][0]);
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f}", mat[0][1], mat[1][1], mat[2][1]);
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f}", mat[0][2], mat[1][2], mat[2][2]);
	separationLine();
}
void Log::log(glm::mat4x4 mat) {
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f} {:.2f}", mat[0][0], mat[1][0], mat[2][0], mat[3][0]);
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f} {:.2f}", mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f} {:.2f}", mat[0][2], mat[1][2], mat[2][2], mat[3][2]);
	spdlog::info("[matrix] {:.2f} {:.2f} {:.2f} {:.2f}", mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
	separationLine();
}