#include "template.h"

extern "C" {
    // log setting (Default is False)
    bool log_enabled = false;

    LogLevel system_thresholds[LOG_MAX] = {LOG_LEVEL_NONE, LOG_LEVEL_NONE, LOG_LEVEL_NONE}; // Default thresholds

    // Switch between Silent Mode Enabled and Disabled
    void setting(){
        if (log_enabled != true){
            log_enabled = true;
        } else {
            log_enabled = false;
        }
    }
    
    void set_system_log_level(LogSubsystem subsystem, LogLevel level) {
        if (subsystem < LOG_MAX) {
            system_thresholds[subsystem] = level;
        }
    }

    const char* get_log_level_string(LogLevel level) {
        switch (level) {
            case LOG_LEVEL_CRITICAL: return "CRITICAL";
            case LOG_LEVEL_ERROR: return "ERROR";
            case LOG_LEVEL_WARNING: return "WARNING";
            case LOG_LEVEL_INFO: return "INFO";
            case LOG_LEVEL_DEBUG: return "DEBUG";
            default: return "NONE";
        }
    }

    const char* get_log_subsystem_string(LogSubsystem subsystem) {
        switch (subsystem) {
            case LOG_SYSTEM: return "SYSTEM";
            case LOG_NAV: return "NAV";
            case LOG_MOTOR: return "MOTOR";
            default: return "UNKNOWN";
        }
    }

    bool logger(LogSubsystem subsystem, LogLevel level, const char* message){
        if (level > system_thresholds[subsystem]){
            return false;
        }
        printf("System: %s    %s: %s\n", get_log_subsystem_string(subsystem), get_log_level_string(level), message);
        return true;
    }
}