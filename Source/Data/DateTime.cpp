#include "DateTime.h"

#include <sstream>

namespace ClassicLauncher
{
    void DateTime::ValidateDateTime()
    {
        const bool bIsLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        int maxDay = 31;

        switch (month)
        {
            case 4:
            case 6:
            case 9:
            case 11:
                maxDay = 30;
                break;
            case 2:
                maxDay = bIsLeapYear ? 29 : 28;
                break;
        }

        // Validate date and time ranges
        if (month < 1 || month > 12 || day < 1 || day > maxDay || hour < 0 || hour > 23 ||
            minute < 0 || minute > 59 || second < 0 || second > 59)
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
        if (value.size() != 15 || value[8] != 'T' || !std::isdigit(value[0]) ||
            !std::isdigit(value[1]) || !std::isdigit(value[2]) || !std::isdigit(value[3]) ||
            !std::isdigit(value[4]) || !std::isdigit(value[5]) || !std::isdigit(value[6]) ||
            !std::isdigit(value[7]) || !std::isdigit(value[9]) || !std::isdigit(value[10]) ||
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

    DateTime& DateTime::operator=(const char* value) { return *this = std::string{ value }; }

    bool DateTime::operator<(const DateTime& a) const { return CompareDates(a); }

    bool DateTime::operator>(const DateTime& a) const { return a.CompareDates(*this); }

    bool DateTime::operator==(const DateTime& a) const
    {
        return (year == a.year) && (month == a.month) && (day == a.day) && (hour == a.hour) &&
               (minute == a.minute) && (second == a.second);
    }

    bool DateTime::operator!=(const DateTime& a) const { return !(*this == a); }

    bool DateTime::operator>=(const DateTime& a) const { return !(*this < a); }

    bool DateTime::operator<=(const DateTime& a) const { return !(*this > a); }

    bool DateTime::CompareDates(const DateTime& a) const
    {
        if (year != a.year) return year < a.year;
        if (month != a.month) return month < a.month;
        if (day != a.day) return day < a.day;
        if (hour != a.hour) return hour < a.hour;
        if (minute != a.minute) return minute < a.minute;
        if (second != a.second) return second < a.second;
        return false;
    }

    std::string DateTime::FormatDateTimeToXml() const
    {
        std::ostringstream value;
        value << ZeroDigits(year) << ZeroDigits(month) << ZeroDigits(day) << "T" << ZeroDigits(hour)
              << ZeroDigits(minute) << ZeroDigits(second);
        return value.str();
    }

    std::string DateTime::ZeroDigits(const int value)
    {
        return (value < 9) ? "0" + std::to_string(value) : std::to_string(value);
    }

}  // namespace ClassicLauncher