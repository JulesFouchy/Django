#pragma once

#include <filesystem>

namespace MyFile {
	bool Exists(const std::string& filepath);
	std::string GetFullPath(const std::string& filepath);
	void ToString(const std::string& filepath, std::string* dst);

	const std::string RootDir = std::filesystem::current_path().string();
}