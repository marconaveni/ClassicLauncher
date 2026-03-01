#include "SimpleIni.h"

#include <cstdlib> // strtof
#include <fstream>
#include <iostream>
#include <string>

SimpleIni::SimpleIni()
{
}

SimpleIni::~SimpleIni()
{
    m_data.clear();
}

void SimpleIni::RemoveComments(std::string& str)
{
    const std::string strWithComments = str;
    str = "";
    for (const char c : strWithComments)
    {
        if (c == '#' || c == ';')
        {
            break;
        }
        str += c;
    }
}

bool SimpleIni::SetSection(std::string& str)
{
    if (str[0] == '[')
    {
        RemoveComments(str);
        str = Trim(RemoveBrackets(str));
        m_currentSection = str;
        return true;
    }
    return false;
}

bool SimpleIni::SetKeyValue(const std::string& str, std::string& key, std::string& value)
{
    const size_t equal = str.find('=');
    if (equal == std::string::npos)
    {
        return false;
    }

    key = Trim(str.substr(0, equal));
    value = Trim(str.substr(equal + 1));

    return !key.empty() && !value.empty();
}

std::string SimpleIni::Trim(const std::string& str)
{
    const char* especialChars = " \t\r\n";
    const size_t first = str.find_first_not_of(especialChars);
    const size_t last = str.find_last_not_of(especialChars);
    if (first == std::string::npos || last == std::string::npos)
    {
        return "";
    }
    return str.substr(first, (last - first + 1));
}

std::string SimpleIni::RemoveBrackets(const std::string& str)
{
    std::string newstr;
    for (const char c : str)
    {
        if (c == '[')
        {
            continue;
        }
        if (c == ']')
        {
            return newstr;
        }
        newstr += c;
    }
    return newstr;
}

bool SimpleIni::Open(const char* file)
{
    std::ifstream fileInput;
    fileInput.open(file);

    if (!fileInput.is_open())
    {
        std::cout << "open is failed!!\n";
        return false;
    }

    m_data.clear();
    char text[MAX_LINE_TEXT];

    while (fileInput.getline(text, MAX_LINE_TEXT))
    {
        std::string key;
        std::string value;
        std::string str = text;
        if (str.empty() || SetSection(str))
        {
            continue;
        }
        RemoveComments(str);
        if (SetKeyValue(str, key, value))
        {
            SetValue(m_currentSection, key, value);
        }
    }
    fileInput.close();


    return true;
}

bool SimpleIni::Save(const char* file)
{
    if (m_data.empty())
    {
        std::cout << "No settings to save.\n";
        return false;
    }

    std::ofstream fileOut;
    fileOut.open(file);

    if (!fileOut.is_open())
    {
        std::cout << "Error to open file: " << file << "\n";
        return false;
    }

    for (const auto& section : m_data)
    {
        if (section.second.empty())
        {
            continue;
        }

        fileOut << "\n[" << section.first << "]\n";
        for (const auto& key : section.second)
        {
            fileOut << key.first << " = " << key.second << "\n";
        }
    }
    fileOut.close();
    return true;
}

std::string SimpleIni::GetValue(const std::string& section, const std::string& key, const std::string& defaultValue)
{
    if (m_data.count(section) && m_data[section].count(key))
    {
        return m_data[section][key];
    }
    return defaultValue;
}

void SimpleIni::SetValue(const std::string& section, const std::string& key, const std::string& value)
{
    if (key.empty() || section.empty())
    {
        return;
    }

    if (value.empty())
    {
        RemoveValue(section, key, value);
        return;
    }
    m_data[section][key] = value;
}

std::string SimpleIni::GetString(const std::string& section, const std::string& key, const std::string& defaultValue)
{
    return GetValue(section, key, defaultValue);
}

bool SimpleIni::GetBoolean(const std::string& section, const std::string& key, const bool defaultValue)
{
    const std::string value = GetValue(section, key, defaultValue ? "true" : "false");
    return (value == "true");
}

int SimpleIni::GetInt(const std::string& section, const std::string& key, const int defaultValue)
{
    return static_cast<int>(GetFloat(section, key, static_cast<float>(defaultValue)));
}

float SimpleIni::GetFloat(const std::string& section, const std::string& key, const float defaultValue)
{
    std::string value = GetValue(section, key, "");
    if (value.empty() || (value.size() == 1 && value[0] == '-'))
    {
        return defaultValue;
    }

    while (!value.empty() && std::isspace(value.back()))
    {
        value.pop_back();
    }

    char* end;
    const float num = std::strtof(value.c_str(), &end);

    return (*end != '\0') ? defaultValue : num;
}

void SimpleIni::SetString(const std::string& section, const std::string& key, const std::string& value)
{
    SetValue(section, key, value);
}

void SimpleIni::SetBoolean(const std::string& section, const std::string& key, bool value)
{
    SetValue(section, key, value ? "true" : "false");
}

void SimpleIni::SetInt(const std::string& section, const std::string& key, int value)
{
    SetValue(section, key, std::to_string(value));
}

void SimpleIni::SetFloat(const std::string& section, const std::string& key, float value)
{
    SetValue(section, key, std::to_string(value));
}

bool SimpleIni::RemoveValue(const std::string& section, const std::string& key, const std::string& value)
{
    if (m_data.count(section) == 1)
    {
        if (m_data[section].count(key) == 1)
        {
            m_data[section].erase(key);
            return true;
        }
    }
    return false;
}
