#pragma once


#include <string>


class StringFunctionLibrary {

public:


	static std::string NormalizePath(const std::string& path);

	static void ReplaceString (std::string& value, const char* from, const char* to);

	static void ReplaceString (std::string& value, const std::string& from, const std::string& to);

	static std::string RemoveDuplicateSlashes(const std::string& input);
};