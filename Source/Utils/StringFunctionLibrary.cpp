#include "StringFunctionLibrary.h"

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

namespace ClassicLauncher
{

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
                if (!bPreviousIsSlash)
                {
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

    std::vector<std::string> StringFunctionLibrary::SplitString(const std::string& input)
    {
        std::vector<std::string> tokens;
        std::string token;
        bool insideQuotes = false;

        for (const char c : input)
        {
            if (!isspace(static_cast<unsigned char>(c)) || insideQuotes)
            {
                token += c;
                if (c == '\"')
                {
                    insideQuotes = !insideQuotes;
                }
            }
            else
            {
                token = Trim(token);
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
            }
        }

        if (!token.empty())
        {
            tokens.push_back(Trim(token));
        }
        return tokens;
    }

    std::string StringFunctionLibrary::Ltrim(const std::string& s)
    {
        size_t start = 0;
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (!isspace(static_cast<unsigned char>(s[i])))
            {
                start = i;
                break;
            }
        }
        return s.substr(start);
    }

    std::string StringFunctionLibrary::Rtrim(const std::string& s)
    {
        size_t end = s.size();
        for (size_t i = s.size(); i > 0; --i)
        {
            if (!isspace(static_cast<unsigned char>(s[i - 1])))
            {
                end = i;
                break;
            }
        }
        return s.substr(0, end);
    }

    std::string StringFunctionLibrary::Trim(const std::string& s)
    {
        return Ltrim(Rtrim(s));
    }

    bool StringFunctionLibrary::IsAllDigits(const std::string& str)
    {
        for (const char c : str)
        {
            if (!std::isdigit(c))
            {
                return false;
            }
        }
        return true;
    }

}  // namespace ClassicLauncher
