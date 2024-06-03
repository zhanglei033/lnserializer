#include "lntime.h"
#include "lnassert.h"

#if LN_PLATFORM_WIN32
#include <sys/timeb.h>
struct timezone
{
    int tz_minuteswest;
    int tz_dsttime;
};

static int gettimeofday(struct timeval* tv, struct timezone* tz) LN_NOEXCEPT
{
    struct _timeb tb;
    _ftime(&tb);
    if (tv)
    {
        tv->tv_sec  = (long)tb.time;
        tv->tv_usec = tb.millitm * 1000;
    }
    if (tz)
    {
        tz->tz_minuteswest = tb.timezone;
        tz->tz_dsttime     = tb.dstflag;
    }
    return 0;
}
#endif

uint64_t lntime_s() LN_NOEXCEPT
{
    time_t cur_time;
    time(&cur_time);
    return cur_time;
}

uint64_t lntime_ms() LN_NOEXCEPT
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * (uint64_t)1000 + tv.tv_usec / 1000;
}

uint64_t lntime_us() LN_NOEXCEPT
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

uint64_t lntime_start_s() LN_NOEXCEPT
{
#if LN_PLATFORM_WIN32
    static LONGLONG freq = 0;
    if (freq == 0)
    {
        LARGE_INTEGER temp_freq;
        QueryPerformanceFrequency(&temp_freq);
        freq = temp_freq.QuadPart;
    }
    if (freq != 0)
    {
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return (unsigned long long)(count.QuadPart / (double)freq * 1000);
    }
    return 0;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec;
#endif
}

uint64_t lntime_start_ms() LN_NOEXCEPT
{
#if LN_PLATFORM_WIN32
    static LONGLONG freq = 0;
    if (freq == 0)
    {
        LARGE_INTEGER temp_freq;
        QueryPerformanceFrequency(&temp_freq);
        freq = temp_freq.QuadPart;
    }
    if (freq != 0)
    {
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return (unsigned long long)(count.QuadPart / (double)freq * 1000);
    }
    return 0;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * (unsigned long long)1000 + ts.tv_nsec / 1000000;
#endif
}

uint64_t lntime_start_us() LN_NOEXCEPT
{
#if LN_PLATFORM_WIN32
    static LONGLONG freq = 0;
    if (freq == 0)
    {
        LARGE_INTEGER temp_freq;
        QueryPerformanceFrequency(&temp_freq);
        freq = temp_freq.QuadPart;
    }
    if (freq != 0)
    {
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return (unsigned long long)(count.QuadPart / (double)freq * 1000000);
    }
    return 0;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * (unsigned long long)1000000 + ts.tv_nsec / 1000;
#endif
}

lntime_date_t lntime_datetime_now() LN_NOEXCEPT
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_t        seconds = tv.tv_sec;
    struct tm*    tm      = localtime(&seconds);
    lntime_date_t date;
    date.year  = tm->tm_year + 1900;
    date.month = tm->tm_mon + 1;
    date.day   = tm->tm_mday;
    date.hour  = tm->tm_hour;
    date.min   = tm->tm_min;
    date.sec   = tm->tm_sec;
    date.ms    = tv.tv_usec / 1000;
    return date;
}

std::string lntime_datetime_format(const lntime_date_t& date) LN_NOEXCEPT
{
    std::string datetime_str;
    datetime_str.resize(g_lntime_date_format_len);
    sprintf(datetime_str.data(), g_lntime_date_format, date.year, date.month, date.day, date.hour, date.min, date.sec);
    return datetime_str;
}

time_t lntime_cron_next_timeout(int minute, int hour, int day, int week, int month) LN_NOEXCEPT
{
    enum
    {
        SECONDLY,
        MINUTELY,
        HOURLY,
        DAILY,
        WEEKLY,
        MONTHLY,
        YEARLY,
    } period_type = MINUTELY;
    struct tm tm;
    time_t    tt;
    time(&tt);
    tm              = *localtime(&tt);
    time_t tt_round = 0;

    tm.tm_sec = 0;
    if (minute >= 0)
    {
        period_type = HOURLY;
        tm.tm_min   = minute;
    }
    if (hour >= 0)
    {
        period_type = DAILY;
        tm.tm_hour  = hour;
    }
    if (week >= 0)
    {
        period_type = WEEKLY;
    }
    else if (day > 0)
    {
        period_type = MONTHLY;
        tm.tm_mday  = day;
        if (month > 0)
        {
            period_type = YEARLY;
            tm.tm_mon   = month - 1;
        }
    }

    tt_round = mktime(&tm);
    if (week >= 0)
    {
        tt_round += (week - tm.tm_wday) * g_lntime_seconds_day;
    }
    if (tt_round > tt)
    {
        return tt_round;
    }

    switch (period_type)
    {
    case MINUTELY:
        tt_round += g_lntime_seconds_minute;
        return tt_round;
    case HOURLY:
        tt_round += g_lntime_seconds_hour;
        return tt_round;
    case DAILY:
        tt_round += g_lntime_seconds_day;
        return tt_round;
    case WEEKLY:
        tt_round += g_lntime_seconds_week;
        return tt_round;
    case MONTHLY:
        if (++tm.tm_mon == 12)
        {
            tm.tm_mon = 0;
            ++tm.tm_year;
        }
        break;
    case YEARLY:
        ++tm.tm_year;
        break;
    default:
        return -1;
    }

    return mktime(&tm);
}

void lntime_set(uint64_t s, uint64_t us) LN_NOEXCEPT
{
// win32暂时不设置系统时间
#ifndef LN_PLATFORM_WIN32
    struct timeval stime;
    stime.tv_sec  = s;
    stime.tv_usec = us;
    settimeofday(&stime, NULL);
#endif // !LN_PLATFORM_WIN32
}