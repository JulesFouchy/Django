#pragma once

#include <filesystem>

namespace djg {
	const std::string SettingsFolder = std::filesystem::current_path().string() + "/settings";
}