#pragma once

#include <filesystem>

namespace FolderPath {
	const std::string Settings = std::filesystem::current_path().string() + "/settings";
	const std::string Records  = std::filesystem::current_path().string() + "/records";
	const std::string Exports  = std::filesystem::current_path().string() + "/exports";
}