#pragma once

#include <filesystem>
#include "Helper/String.h"

namespace FolderPath {
	const std::string Settings = std::filesystem::current_path().string() + "/settings";
	const std::string Records  = std::filesystem::current_path().string() + "/records";
	const std::string Exports  = MyString::ReplaceAll(std::filesystem::current_path().string(), "\\", "/") + "/exports";
}