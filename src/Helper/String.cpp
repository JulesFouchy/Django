#include "String.h"

std::string MyString::ToLower(const std::string& str) {
	std::string res = "";
	std::for_each(str.begin(), str.end(), [&res](char c) {
		res += std::tolower(c);
	});
	return res;
}

std::string MyString::RemoveFolderHierarchy(const std::string& myString) {
	auto pos = myString.find_last_of("/\\") + 1;
	return myString.substr(pos, myString.size() - pos);
}

std::string MyString::RemoveFileExtension(const std::string& myString) {
	auto pos = myString.find_last_of(".");
	return myString.substr(0, pos);
}

std::string MyString::FileName(const std::string& myString) {
	return RemoveFolderHierarchy(RemoveFileExtension(myString));
}

std::string MyString::FileExtension(const std::string& myString) {
	auto pos = myString.find_last_of(".");
	return myString.substr(pos, myString.size());
}