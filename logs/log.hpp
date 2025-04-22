#pragma once

#include <stdint.h>
#include <stdarg.h>   // for va_start, va_end

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_NONE
} LogLevel;

void log_set_level(LogLevel level);
void log_debug(const char* format, ...);
void log_info(const char* format, ...);
void log_warn(const char* format, ...);
void log_error(const char* format, ...);

#define LOG_DEBUG(...) log_debug(__VA_ARGS__)
#define LOG_INFO(...)  log_info(__VA_ARGS__)
#define LOG_WARN(...)  log_warn(__VA_ARGS__)
#define LOG_ERROR(...) log_error(__VA_ARGS__)
