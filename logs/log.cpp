#include "log.hpp"
#include <stdio.h>

static LogLevel current_log_level = LOG_DEBUG;

void log_set_level(LogLevel level)
{
    current_log_level = level;
}

static void log_output(const char* color_code, const char* level, const char* format, va_list args)
{
    printf("%s[%s] ", color_code, level);
    vprintf(format, args);
    printf("\033[0m\n");
}

void log_debug(const char* format, ...)
{
    if (current_log_level <= LOG_DEBUG)
    {
        va_list args;
        va_start(args, format);
        log_output("\033[36m", "DEBUG", format, args);
        va_end(args);
    }
}

void log_info(const char* format, ...)
{
    if (current_log_level <= LOG_INFO)
    {
        va_list args;
        va_start(args, format);
        log_output("\033[32m", "INFO ", format, args);
        va_end(args);
    }
}

void log_warn(const char* format, ...)
{
    if (current_log_level <= LOG_WARN)
    {
        va_list args;
        va_start(args, format);
        log_output("\033[33m", "WARN ", format, args);
        va_end(args);
    }
}

void log_error(const char* format, ...)
{
    if (current_log_level <= LOG_ERROR)
    {
        va_list args;
        va_start(args, format);
        log_output("\033[31m", "ERROR", format, args);
        va_end(args);
    }
}
