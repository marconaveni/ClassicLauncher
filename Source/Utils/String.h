#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

namespace ClassicLauncher::String
{

    std::string NormalizePath(const std::string& path);
    void ReplaceString(std::string& value, const char* from, const char* to);
    void ReplaceString(std::string& value, const std::string& from, const std::string& to);
    std::string RemoveDuplicateSlashes(const std::string& input);
    std::vector<std::string> SplitString(const std::string& input);
    std::string Ltrim(const std::string& str);
    std::string Rtrim(const std::string& str);
    std::string Trim(const std::string& str);
    bool IsIntegerNumber(const std::string& str);
    const char* TextFormat(const char* text, ...);

} // namespace ClassicLauncher::String

#endif
