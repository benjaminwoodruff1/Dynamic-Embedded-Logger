#ifndef TEMPLATE_H
#define TEMPLATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

// Define Verbosity Levels for logging
typedef enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_CRITICAL = 1,
    LOG_LEVEL_ERROR = 2,
    LOG_LEVEL_WARNING = 3,
    LOG_LEVEL_INFO = 4,
    LOG_LEVEL_DEBUG = 5,
} LogLevel;

// Define Loggers/Subsystems
typedef enum
{
    LOG_SYSTEM = 0,
    LOG_NAV = 1,
    LOG_MOTOR = 2,
    LOG_MAX
} LogSubsystem;

extern LogLevel system_thresholds[LOG_MAX];

void set_system_log_level(LogSubsystem subsystem, LogLevel level);
const char* get_log_level_string(LogLevel level);
const char* get_log_subsystem_string(LogSubsystem subsystem);

// Inline function to log messages based on state of log_info
// Being inlined, this function will elminate function call overhead when log_info is false/disabled
static inline void log_info_inline(LogSubsystem subsystem, LogLevel level, const char* format, ...) {
    if (level <= system_thresholds[subsystem]) {
        va_list args;
        va_start(args, format);
        printf("System: %s", get_log_subsystem_string(subsystem));
        printf("    %s: ", get_log_level_string(level));
            //        printf("[%s] [%s]: ", get_log_subsystem_string(subsystem), get_log_level_string(level));
            vprintf(format, args);
        printf("\n");
        va_end(args);
    }
}

#ifdef __cplusplus
}
#endif

#endif