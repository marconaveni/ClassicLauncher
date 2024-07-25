#pragma once
#include <chrono>
#include <string>

struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

private:

    std::tm currentTime;

public:
    DateTime()
	    : year(1900),
	      month(1),
	      day(1),
	      hour(0),
	      minute(0),
	      second(0),
	      currentTime()
    {
    }

    void GetCurrentTimeAndDate()
	{
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        currentTime = *std::localtime(&in_time_t);

        year = currentTime.tm_year + 1900; // add 1900 in year
        month = currentTime.tm_mon + 1;    // month start with 0
        day = currentTime.tm_mday;
        hour = currentTime.tm_hour;
        minute = currentTime.tm_min;
        second = currentTime.tm_sec;

    }

    DateTime& operator=(const std::string& value);
    DateTime& operator=(const char* value);
    bool operator<(const DateTime& a) const;
    bool operator>(const DateTime& a) const;
    bool operator==(const DateTime& a) const;
    bool operator!=(const DateTime& a) const;
    bool operator>=(const DateTime& a) const;
    bool operator<=(const DateTime& a) const;

    std::string FormatDateTimeToXml() const;

private:

    bool CompareDates(const DateTime& a) const;

    void ValidateDateTime();

    static std::string ZeroDigits(const int value);

};





