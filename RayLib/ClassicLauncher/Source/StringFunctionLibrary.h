#pragma once


#include <string>
#include <vector>


class StringFunctionLibrary {

public:


	static std::string NormalizePath(const std::string& path);

	static void ReplaceString (std::string& value, const char* from, const char* to);

	static void ReplaceString (std::string& value, const std::string& from, const std::string& to);

	static std::string RemoveDuplicateSlashes(const std::string& input);

	static std::vector<std::string> SplitString(const std::string& input);

	static std::string Ltrim(const std::string& s);

	static std::string Rtrim(const std::string& s);

	static std::string Trim(const std::string& s);

};