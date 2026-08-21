#ifndef INI_H
#define INI_H

#include <filesystem>
#include <memory>
#include <string>


namespace ClassicLauncher
{
    
    struct IniData;

    class Ini
    {

    public:

        Ini();
        ~Ini();

        bool Open(const std::filesystem::path file);
        bool Save(const std::filesystem::path file);

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

        std::string GetValue(const std::string& section, const std::string& key, const std::string& defaultValue = "");
        void SetValue(const std::string& section, const std::string& key, const std::string& value, const std::string& comments = "");

        std::unique_ptr<IniData> m_data{};
    };

} // namespace ClassicLauncher

#endif
