#pragma once

namespace MyString {
	std::string ToLower(const std::string& str);
	std::string RemoveFolderHierarchy(const std::string& myString);
	std::string RemoveFileExtension(const std::string& myString);
	std::string FileName(const std::string& myString);
	std::string FileExtension(const std::string& myString);
}