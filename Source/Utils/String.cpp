#include "String.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

namespace ClassicLauncher
{

    std::string String::NormalizePath(const std::string& path)
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

    void String::ReplaceString(std::string& value, const char* from, const char* to)
    {
        ReplaceString(value, std::string{from}, std::string{to});
    }

    void String::ReplaceString(std::string& value, const std::string& from, const std::string& to)
    {
        if (from.empty())
        {
            return;
        }

        size_t startPos = 0;
        while ((startPos = value.find(from, startPos)) != std::string::npos)
        {
            value.replace(startPos, from.length(), to);
            startPos += to.length();
        }
    }

    std::string String::RemoveDuplicateSlashes(const std::string& input)
    {
        std::string result;
        bool isPreviousIsSlash = false;

        for (char c : input)
        {
#ifdef _WIN32
            if (c == '\\')
#else
            if (c == '/')
#endif

            {
                if (!isPreviousIsSlash)
                {
                    result += c;
                    isPreviousIsSlash = true;
                }
            }
            else
            {
                result += c;
                isPreviousIsSlash = false;
            }
        }

        return result;
    }

    std::vector<std::string> String::SplitString(const std::string& input)
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

    std::string String::Ltrim(const std::string& str)
    {
        size_t start = 0;
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (!isspace(static_cast<unsigned char>(str[i])))
            {
                start = i;
                break;
            }
        }
        return str.substr(start);
    }

    std::string String::Rtrim(const std::string& str)
    {
        size_t end = str.size();
        for (size_t i = str.size(); i > 0; --i)
        {
            if (!isspace(static_cast<unsigned char>(str[i - 1])))
            {
                end = i;
                break;
            }
        }
        return str.substr(0, end);
    }

    std::string String::Trim(const std::string& str)
    {
        return Ltrim(Rtrim(str));
    }

    bool String::IsIntegerNumber(const std::string& str)
    {
        if (str.empty() || (str.size() == 1 && str[0] == '-'))
        {
            return false;
        }

        const std::string strTrimmed = Trim(str);
        int start = (strTrimmed[0] == '-') ? 1 : 0;

        for (size_t i = start; i < strTrimmed.size(); i++)
        {
            if (!std::isdigit(static_cast<unsigned char>(strTrimmed[i])))
            {
                return false;
            }
        }
        return true;
    }

    const char* String::TextFormat(const char* text, ...)
    {
        const int maxTextFormatBuffers = 4; // Maximum number of static buffers for text formatting
        const int maxTextBufferLen = 1024;

        static char buffers[maxTextFormatBuffers][maxTextBufferLen] = {0};
        static int index = 0;

        char* currentBuffer = buffers[index];
        memset(currentBuffer, 0, maxTextBufferLen);

        std::va_list args;
        va_start(args, text);
        int requiredByteCount = std::vsnprintf(currentBuffer, maxTextBufferLen, text, args);
        va_end(args);

        if (requiredByteCount >= maxTextBufferLen)
        {
            char* truncBuffer = buffers[index] + maxTextBufferLen - 4; // Adding 4 bytes = "...\0"
            std::sprintf(truncBuffer, "...");
        }

        index += 1; // Move to next buffer for next function call
        if (index >= maxTextFormatBuffers)
        {
            index = 0;
        }

        return currentBuffer;
    }

} // namespace ClassicLauncher
