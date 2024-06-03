/*
 * @file xlib_assert.h
 * @author ZhangLei (2604960361@qq.com)
 * @brief 断言
 * @version 0.1
 * @date 2024-04-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "lnlog.h"
#include "lnpp.h"
#if LN_APP_NDEBUG
#define LN_ASSERT(expr, message)
#else
#define LN_ASSERT(expr, message)                                                                                     \
    {                                                                                                                \
        if (!(expr))                                                                                                 \
        {                                                                                                            \
            LN_LOGC("assert", "\"" #expr "\""                                                                        \
                              "file:" LN_PP_STRING(__FILE__) ", line " LN_PP_STRING(__LINE__) ",message: " message); \
            abort();                                                                                                 \
        }                                                                                                            \
    }
#endif