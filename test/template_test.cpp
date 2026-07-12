#include "template.h"
#include <stdio.h>

int main() {
    printf("--- Running Logger System Test ---\n");
    set_system_log_level(LOG_NAV, LOG_LEVEL_INFO);
    logger(LOG_NAV, LOG_LEVEL_DEBUG, "This is a debug message and should not be printed");
    logger(LOG_NAV, LOG_LEVEL_INFO, "This is an info message and should be printed");
    logger(LOG_NAV, LOG_LEVEL_WARNING, "This is a warning message and should be printed");
    logger(LOG_NAV, LOG_LEVEL_ERROR, "This is an error message and should be printed");
    logger(LOG_NAV, LOG_LEVEL_CRITICAL, "This is a critical message and should be printed");
    printf("--- Logger System Test Completed ---\n");
    return 0;
}