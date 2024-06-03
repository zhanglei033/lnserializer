#pragma once
#include "ln3rd.h"
#define LN_LOGC(tag, format, ...) spdlog::critical("{}:" format, tag, ##__VA_ARGS__)
#define LN_LOGE(tag, format, ...) spdlog::error("{}:" format, tag, ##__VA_ARGS__)
#define LN_LOGW(tag, format, ...) spdlog::warn("{}:" format, tag, ##__VA_ARGS__)
#define LN_LOGI(tag, format, ...) spdlog::info("{}:" format, tag, ##__VA_ARGS__)
#define LN_LOGD(tag, format, ...) spdlog::debug("{}:" format, tag, ##__VA_ARGS__)
#define LN_LOGV(tag, format, ...) spdlog::trace("{}:" format, tag, ##__VA_ARGS__)