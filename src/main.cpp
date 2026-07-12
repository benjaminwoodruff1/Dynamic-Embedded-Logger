#include <stdio.h>
#include "template.h"
#include "template_config.h"

void high_frequency_loop() {
    // log_enabled is checked first. If log_enabled is false, && operator doesn't check check(),
    // thus using near zero CPU cycles while log_enabled is disabled
    log_enabled && logger(LOG_NAV, LOG_LEVEL_INFO, "Position updated");
    log_enabled && logger(LOG_NAV, LOG_LEVEL_CRITICAL, "Position exposed");
}

int main(int argc, char** argv) {
    printf("Hello, world!\n");
    printf("Version: %d.%d.%d\n", TEMPLATE_VERSION_MAJOR, TEMPLATE_VERSION_MINOR, TEMPLATE_VERSION_PATCH);
    
    printf("--- Project ---\n");
    printf("Default Silent Mode\n"); //Initial state
    high_frequency_loop();
    printf("DONE.\n\n");

    setting(); // Enable the logging display
    
    set_system_log_level(LOG_NAV, LOG_LEVEL_DEBUG);

    printf("Silent Mode Disabled\n");
    high_frequency_loop();
    set_system_log_level(LOG_NAV, LOG_LEVEL_CRITICAL);
    high_frequency_loop();
    printf("DONE.\n\n");

    setting(); // Disable the logging display
    
    printf("Silent Mode Enabled\n");
    high_frequency_loop();
    printf("DONE.\n");
    return 0;
}
