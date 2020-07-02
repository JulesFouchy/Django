#include "String.h"

std::string MyString::ToLower(const std::string& str) {
	std::string res = "";
	std::for_each(str.begin(), str.end(), [&res](char c) {
		res += std::tolower(c);
	});
	return res;
}