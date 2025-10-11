#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <chrono>
#include <string>

namespace ClassicLauncher
{

    struct DateTime
    {
        int year{1900};
        int month{1};
        int day{1};
        int hour{0};
        int minute{0};
        int second{0};

    private:

        std::tm currentTime{};

    public:

        DateTime() = default;

        void GetCurrentTimeAndDate()
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

        DateTime& operator=(const std::string& value);
        DateTime& operator=(const char* value) { return *this = std::string{value}; };
        bool operator<(const DateTime& a) const { return CompareDates(a); };
        bool operator>(const DateTime& a) const { return a.CompareDates(*this); };
        bool operator!=(const DateTime& a) const { return !(*this == a); };
        bool operator>=(const DateTime& a) const { return !(*this < a); };
        bool operator<=(const DateTime& a) const { return !(*this > a); };
        bool operator==(const DateTime& a) const
        {
            return (year == a.year) && (month == a.month) && (day == a.day) && (hour == a.hour) &&
                   (minute == a.minute) && (second == a.second);
        };

        std::string FormatDateTimeToXml() const;

    private:

        bool CompareDates(const DateTime& a) const;
        void ValidateDateTime();
        static std::string ZeroDigits(const int value);
    };

} // namespace ClassicLauncher

#endif