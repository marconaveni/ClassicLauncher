#include "Ini.h"

#include <cmath>
#include <cstdlib> // strtof
#include <fstream>
#include <iostream>
#include <string>

#include "simpleini/SimpleIni.h"

namespace ClassicLauncher
{

    struct IniData
    {
        CSimpleIniA ini;
    };

    Ini::Ini()
        : m_data(std::make_unique<IniData>())
    {
    }

    Ini::~Ini()
    {
    }

    bool Ini::Open(const std::filesystem::path file)
    {
        m_data->ini.SetUnicode();

        SI_Error rc = m_data->ini.LoadFile(file.string().c_str());
        if (rc < 0)
        {
            std::cout << "File not loaded\n";
            return false;
        }
        return true;
    }

    bool Ini::Save(const std::filesystem::path file)
    {
        SI_Error rc = m_data->ini.SaveFile(file.string().c_str());
        if (rc < 0)
        {
            std::cout << "File not Saved\n";
            return false;
        }
        return true;
    }

    std::string Ini::GetValue(const std::string& section, const std::string& key, const std::string& defaultValue)
    {
        std::string pv;
        pv = m_data->ini.GetValue(section.c_str(), key.c_str(), defaultValue.c_str());
        return pv;
    }

    void Ini::SetValue(const std::string& section, const std::string& key, const std::string& value, const std::string& comments)
    {

        if (m_data->ini.KeyExists(section.c_str(), key.c_str()))
        {
            m_data->ini.SetValue(section.c_str(), key.c_str(), value.c_str());
        }
        else
        {
            m_data->ini.SetValue(section.c_str(), key.c_str(), value.c_str(), comments.c_str());
        }
    }

    std::string Ini::GetString(const std::string& section, const std::string& key, const std::string& defaultValue)
    {
        const std::string str = GetValue(section, key, defaultValue);
        return str.empty() ? defaultValue : str;
    }

    bool Ini::GetBoolean(const std::string& section, const std::string& key, const bool defaultValue)
    {
        const std::string value = GetValue(section, key, defaultValue ? "true" : "false");
        return (value == "true");
    }

    int Ini::GetInt(const std::string& section, const std::string& key, const int defaultValue)
    {
        return static_cast<int>(GetFloat(section, key, static_cast<float>(defaultValue)));
    }

    float Ini::GetFloat(const std::string& section, const std::string& key, const float defaultValue)
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

    void Ini::SetString(const std::string& section, const std::string& key, const std::string& value, const std::string& comments)
    {
        SetValue(section, key, value, comments);
    }

    void Ini::SetBoolean(const std::string& section, const std::string& key, bool value, const std::string& comments)
    {
        SetValue(section, key, value ? "true" : "false", comments);
    }

    void Ini::SetInt(const std::string& section, const std::string& key, int value, const std::string& comments)
    {
        SetValue(section, key, std::to_string(value), comments);
    }

    void Ini::SetFloat(const std::string& section, const std::string& key, float value, const std::string& comments)
    {
        SetValue(section, key, std::to_string(value), comments);
    }

    bool Ini::RemoveValue(const std::string& section, const std::string& key, const std::string& value)
    {
        bool done, deleteSectionIfEmpty = true;
        done = m_data->ini.Delete(section.c_str(), key.c_str(), deleteSectionIfEmpty);
        return done;
    }

} // namespace ClassicLauncher
