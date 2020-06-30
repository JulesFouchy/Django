#include "File.h"

#include <sys/stat.h>

bool MyFile::Exists(const std::string& filepath) {
	struct stat buffer;
	return (stat(filepath.c_str(), &buffer) == 0);
}

std::string MyFile::GetFullPath(const std::string& filepath) {
	return std::filesystem::absolute(filepath).string();
}