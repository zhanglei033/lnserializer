#pragma once
#include "lnplatform.h"

inline constexpr auto g_lntime_weekdays       = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
inline constexpr auto g_lntime_months         = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
inline constexpr auto g_lntime_days           = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
inline constexpr auto g_lntime_seconds_minute = 60;     // 一分钟有60秒
inline constexpr auto g_lntime_seconds_hour   = 3600;   // 一分钟有3600秒
inline constexpr auto g_lntime_seconds_day    = 86400;  // 一天有86400秒
inline constexpr auto g_lntime_seconds_week   = 604800; // 一天有604800秒

inline constexpr auto g_lntime_clock_format_len = 12;                                         // 时间格式长度
inline constexpr auto g_lntime_clock_format     = "%02d:%02d:%02d ";                          // 时间格式
inline constexpr auto g_lntime_date_format_len  = 30;                                         // 时间格式长度
inline constexpr auto g_lntime_date_format      = "%04d-%02d-%02d %02d:%02d:%02d ";           // 日期格式
inline constexpr auto g_lntime_date_format_iso  = "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ ";     // ISO日期格式
inline constexpr auto g_lntime_gmt_format_len   = 30;                                         // GMT时间格式长度
inline constexpr auto g_lntime_gmt_format       = "%.3s, %02d %.3s %04d %02d:%02d:%02d GMT "; // GMT时间格式

inline constexpr LN_NODISCARD bool lntime_leap_year(int year) LN_NOEXCEPT
{
    return (((year) % 4 == 0 && (year) % 100 != 0) || (year) % 400 == 0);
}

struct lntime_date_t
{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int ms;
};

/*
 * @brief
 * @return uint64_t 格林尼治时间1970年1月1日00:00:00到当前时刻的时长，时长单位是秒
 */
LN_NODISCARD uint64_t lntime_s() LN_NOEXCEPT;

/*
 * @brief
 * @return uint64_t 格林尼治时间1970年1月1日00:00:00到当前时刻的时长，时长单位是秒
 */
LN_NODISCARD uint64_t lntime_ms() LN_NOEXCEPT;

/*
 * @brief
 * @return uint64_t 格林尼治时间1970年1月1日00:00:00到当前时刻的时长，时长单位是秒
 */
LN_NODISCARD uint64_t lntime_us() LN_NOEXCEPT;

/*
 * @brief 系统启动后到现在的秒，不随用户修改系统时间而改变
 *
 * @return uint64_t
 */
LN_NODISCARD uint64_t lntime_start_s() LN_NOEXCEPT;
/*
 * @brief 系统启动后到现在的毫秒，不随用户修改系统时间而改变
 *
 * @return uint64_t
 */
LN_NODISCARD uint64_t lntime_start_ms() LN_NOEXCEPT;
/*
 * @brief 系统启动后到现在的微秒，不随用户修改系统时间而改变
 *
 * @return uint64_t
 */
LN_NODISCARD uint64_t lntime_start_us() LN_NOEXCEPT;

/*
 * @brief 当前日期
 *
 * @return lntime_date_t
 */
LN_NODISCARD lntime_date_t lntime_datetime_now() LN_NOEXCEPT;

/*
 * @brief 格式化日期
 *
 * @param date
 * @return LN_NODISCARD
 */
LN_NODISCARD std::string lntime_datetime_format(const lntime_date_t& date) LN_NOEXCEPT;

/*
 * minute   hour    day     week    month       action
 * 0~59     0~23    1~31    0~6     1~12
 *  -1      -1      -1      -1      -1          cron.minutely
 *  30      -1      -1      -1      -1          cron.hourly
 *  30      1       -1      -1      -1          cron.daily
 *  30      1       15      -1      -1          cron.monthly
 *  30      1       -1       0      -1          cron.weekly
 *  30      1        1      -1      10          cron.yearly
 */
LN_NODISCARD time_t lntime_cron_next_timeout(int minute, int hour, int day, int week, int month) LN_NOEXCEPT;

/*
 * @brief 设置系统时间
 *
 */
void lntime_set(uint64_t s, uint64_t us) LN_NOEXCEPT;
