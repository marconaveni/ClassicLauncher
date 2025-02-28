#ifndef STRING_FUNCTION_LIBRARY_H
#define STRING_FUNCTION_LIBRARY_H

#include <string>
#include <vector>

namespace ClassicLauncher::StringFunctionLibrary
{

    std::string NormalizePath(const std::string& path);
    void ReplaceString(std::string& value, const char* from, const char* to);
    void ReplaceString(std::string& value, const std::string& from, const std::string& to);
    std::string RemoveDuplicateSlashes(const std::string& input);
    std::vector<std::string> SplitString(const std::string& input);
    std::string Ltrim(const std::string& s);
    std::string Rtrim(const std::string& s);
    std::string Trim(const std::string& s);
    bool IsIntegerNumber(const std::string& str);

}  // namespace ClassicLauncher::StringFunctionLibrary

#endif
