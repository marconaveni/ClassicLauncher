#ifndef SIMPLE_INI_H
#define SIMPLE_INI_H


#include <string>
#include <map>

#define MAX_LINE_TEXT 200

class SimpleIni
{
private:

    std::map<std::string, std::map<std::string, std::string>> mData;
    std::string mCurrentSection = "Default";
    void RemoveComments(std::string& str);
    bool SetSection(std::string& str);
    bool SetKeyValue(const std::string& str, std::string& key, std::string& value);
    std::string Trim(const std::string& str);
    std::string RemoveBrackets(const std::string& str);
    std::string GetValue(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    void SetValue(const std::string& section, const std::string& key, const std::string& value);

public:

    SimpleIni();
    ~SimpleIni();
    
    bool Open(const char* file);
    bool Save(const char* file);

    std::string GetString(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    bool GetBoolean(const std::string& section, const std::string& key, bool defaultValue = false);
    int GetInt(const std::string& section, const std::string& key, int defaultValue = 0);
    float GetFloat(const std::string& section, const std::string& key, float defaultValue = 0.0f);
    
    void SetString(const std::string& section, const std::string& key, const std::string& value);
    void SetBoolean(const std::string& section, const std::string& key, bool value);
    void SetInt(const std::string& section, const std::string& key, int value);
    void SetFloat(const std::string& section, const std::string& key, float value);

    bool RemoveValue(const std::string& section, const std::string& key, const std::string& value);
};

#endif
