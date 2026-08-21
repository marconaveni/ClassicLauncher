#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <chrono>
#include <string>

namespace ClassicLauncher
{

    class DateTime
    {
    public:

        int year{1900};
        int month{1};
        int day{1};
        int hour{0};
        int minute{0};
        int second{0};

        DateTime() = default;
        DateTime& operator=(const std::string& value);
        DateTime& operator=(const char* value) { return *this = std::string{value}; };
        bool operator<(const DateTime& a) const { return CompareDates(a); };
        bool operator>(const DateTime& a) const { return a.CompareDates(*this); };
        bool operator!=(const DateTime& a) const { return !(*this == a); };
        bool operator>=(const DateTime& a) const { return !(*this < a); };
        bool operator<=(const DateTime& a) const { return !(*this > a); };
        bool operator==(const DateTime& a) const
        {
            return (year == a.year) && (month == a.month) && (day == a.day) && (hour == a.hour) && (minute == a.minute) && (second == a.second);
        };

        void SetCurrentTimeAndDate();
        std::string ToXmlString() const;
        std::string ToLocaleString() const;

    private:

        bool CompareDates(const DateTime& dateTime) const;
        void ValidateDateTime();
        static std::string ZeroDigits(const int value);

        std::tm currentTime{};
    };

} // namespace ClassicLauncher

#endif