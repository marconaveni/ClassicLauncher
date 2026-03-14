#ifndef SIMPLE_INI_H
#define SIMPLE_INI_H


#include <map>
#include <unordered_map>
#include <string>

#define MAX_LINE_TEXT 200

class SimpleIni
{
public:

    SimpleIni();
    ~SimpleIni();

    bool Open(const char* file);
    bool Save(const char* file);

    std::string GetString(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    bool GetBoolean(const std::string& section, const std::string& key, bool defaultValue = false);
    int GetInt(const std::string& section, const std::string& key, int defaultValue = 0);
    float GetFloat(const std::string& section, const std::string& key, float defaultValue = 0.0f);

    void SetString(const std::string& section, const std::string& key, const std::string& value, const std::string& comments = "");
    void SetBoolean(const std::string& section, const std::string& key, bool value, const std::string& comments = "");
    void SetInt(const std::string& section, const std::string& key, int value, const std::string& comments = "");
    void SetFloat(const std::string& section, const std::string& key, float value, const std::string& comments = "");

    bool RemoveValue(const std::string& section, const std::string& key, const std::string& value);

private:

    void SepareComments(std::string& value, std::string& comments);
    bool SetSection(std::string& str);
    bool SetKeyValue(const std::string& str, std::string& key, std::string& value);
    std::string Trim(const std::string& str);
    std::string RemoveBrackets(const std::string& str);
    std::string GetValue(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    void SetValue(const std::string& section, const std::string& key, const std::string& value, const std::string& comments = "");

    struct IniData
    {
        std::string value{};
        std::string comments{};
    };
    
    std::map<std::string, std::map<std::string, IniData>> m_data{};
    std::string m_currentSection{"default"};
};

#endif
