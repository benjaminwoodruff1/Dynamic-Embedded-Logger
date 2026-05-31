#include <stdio.h>
#include "template.h"
#include "template_config.h"

void high_frequency_loop() {
    log_info_inline(LOG_NAV, LOG_LEVEL_DEBUG, "GPS satellites locked: %d", 5);
    log_info_inline(LOG_NAV, LOG_LEVEL_INFO, "Current speed: %.2f m/s", 12.34);

    log_info_inline(LOG_MOTOR, LOG_LEVEL_WARNING, "Motor temperature high: %.1f C", 85.5);
    log_info_inline(LOG_MOTOR, LOG_LEVEL_ERROR, "Motor failure detected!");
}

int main(int argc, char** argv) {
    printf("Hello, world!\n");
    printf("Version: %d.%d.%d\n", TEMPLATE_VERSION_MAJOR, TEMPLATE_VERSION_MINOR, TEMPLATE_VERSION_PATCH);
    
    printf("--- Project ---\n");

    printf("Default Silent Mode\n");
    high_frequency_loop();
    printf("DONE.\n\n");

    set_system_log_level(LOG_NAV, LOG_LEVEL_DEBUG);
    set_system_log_level(LOG_MOTOR, LOG_LEVEL_WARNING);

    printf("Silent Mode Disabled for NAV and MOTOR\n");
    high_frequency_loop();
    printf("DONE.\n");

    return 0;
}
