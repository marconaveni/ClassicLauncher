#include "StringFunctionLibrary.h"

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>




std::string StringFunctionLibrary::NormalizePath(const std::string& path)
{
	std::string convertedPath = path;
#ifdef _WIN32
	ReplaceString(convertedPath, "/", "\\");
#else
	ReplaceString(convertedPath, "\\", "/");
#endif

	convertedPath = RemoveDuplicateSlashes(convertedPath);
	return convertedPath;
}


void StringFunctionLibrary::ReplaceString(std::string& value, const char* from, const char* to)
{
	ReplaceString(value, std::string{ from }, std::string{ to });
}

void StringFunctionLibrary::ReplaceString(std::string& value, const std::string& from, const std::string& to)
{
	if (from.empty()) return;

	size_t startPos = 0;
	while ((startPos = value.find(from, startPos)) != std::string::npos)
	{
		value.replace(startPos, from.length(), to);
		startPos += to.length();
	}
}

std::string StringFunctionLibrary::RemoveDuplicateSlashes(const std::string& input)
{
	std::string result;
	bool bPreviousIsSlash = false;

	for (char c : input) 
	{
#ifdef _WIN32
		if (c == '\\')
#else
		if (c == '/')
#endif
		
		{
			if (!bPreviousIsSlash) {
				result += c;
				bPreviousIsSlash = true;
			}
		}
		else 
		{
			result += c;
			bPreviousIsSlash = false;
		}
	}

	return result;
}



