#include "DateTime.h"

#include <sstream>
#include <clocale>

namespace ClassicLauncher
{
    void DateTime::ValidateDateTime()
    {
        const bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        int maxDay = 31;

        switch (month)
        {
            case 4:
            case 6:
            case 9:
            case 11: maxDay = 30; break;
            case 2: maxDay = isLeapYear ? 29 : 28; break;
        }

        // Validate date and time ranges
        if (month < 1 || month > 12 || day < 1 || day > maxDay || hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
            second < 0 || second > 59)
        {
            // Reset to default values if validation fails
            year = 1900;
            month = 1;
            day = 1;
            hour = 0;
            minute = 0;
            second = 0;
        }
    }

    DateTime& DateTime::operator=(const std::string& value)
    {
        if (value.size() != 15 || value[8] != 'T' || !std::isdigit(value[0]) || !std::isdigit(value[1]) ||
            !std::isdigit(value[2]) || !std::isdigit(value[3]) || !std::isdigit(value[4]) || !std::isdigit(value[5]) ||
            !std::isdigit(value[6]) || !std::isdigit(value[7]) || !std::isdigit(value[9]) || !std::isdigit(value[10]) ||
            !std::isdigit(value[11]) || !std::isdigit(value[12]) || !std::isdigit(value[13]) ||
            !std::isdigit(value[14]))
        {
            return *this;
        }

        year = std::stoi(value.substr(0, 4));
        month = std::stoi(value.substr(4, 2));
        day = std::stoi(value.substr(6, 2));
        hour = std::stoi(value.substr(9, 2));
        minute = std::stoi(value.substr(11, 2));
        second = std::stoi(value.substr(13, 2));

        ValidateDateTime();
        return *this;
    }

    bool DateTime::CompareDates(const DateTime& dateTime) const
    {
        if (year != dateTime.year)
        {
            return year < dateTime.year;
        }
        if (month != dateTime.month)
        {
            return month < dateTime.month;
        }
        if (day != dateTime.day)
        {
            return day < dateTime.day;
        }
        if (hour != dateTime.hour)
        {
            return hour < dateTime.hour;
        }
        if (minute != dateTime.minute)
        {
            return minute < dateTime.minute;
        }
        if (second != dateTime.second)
        {
            return second < dateTime.second;
        }
        return false;
    }

    void DateTime::SetCurrentTimeAndDate()
    {
        const auto now = std::chrono::system_clock::now();
        const auto inTimeT = std::chrono::system_clock::to_time_t(now);
        currentTime = *std::localtime(&inTimeT);

        year = currentTime.tm_year + 1900; // add 1900 in year
        month = currentTime.tm_mon + 1;    // month start with 0
        day = currentTime.tm_mday;
        hour = currentTime.tm_hour;
        minute = currentTime.tm_min;
        second = currentTime.tm_sec;
    }

    std::string DateTime::ToXmlString() const
    {
        std::ostringstream value;
        value << ZeroDigits(year) << ZeroDigits(month) << ZeroDigits(day) << "T" << ZeroDigits(hour)
              << ZeroDigits(minute) << ZeroDigits(second);
        return value.str();
    }

    std::string DateTime::ToLocaleString() const
    {
        std::tm timeinfo{};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = minute;
        timeinfo.tm_sec = second;

        static bool localeSet = []
        {
            setlocale(LC_TIME, "");
            return true;
        }();

        char formatedDate[64];

        std::strftime(formatedDate, sizeof(formatedDate), "%x %X", &timeinfo);

        return std::string(formatedDate);
    }

    std::string DateTime::ZeroDigits(const int value)
    {
        return (value < 10) ? "0" + std::to_string(value) : std::to_string(value);
    }

} // namespace ClassicLauncher