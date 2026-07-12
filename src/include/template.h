#ifndef TEMPLATE_H
#define TEMPLATE_H

#ifdef __cplusplus
extern "C" {
#endif
    #include <stdio.h>

    extern bool log_enabled;

    void setting();

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

    const char *get_log_level_string(LogLevel level);

    const char *get_log_subsystem_string(LogSubsystem subsystem);

    bool logger(LogSubsystem subsystem, LogLevel level, const char* message);

#ifdef __cplusplus
}
#endif

#endif