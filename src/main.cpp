#include <stdio.h>
#include "template.h"
#include "template_config.h"

void high_frequency_loop() {
    log_enabled && check();
}

int main(int argc, char** argv) {
    printf("Hello, world!\n");
    printf("Version: %d.%d.%d\n", TEMPLATE_VERSION_MAJOR, TEMPLATE_VERSION_MINOR, TEMPLATE_VERSION_PATCH);
    
    printf("--- Project ---\n");

    printf("Default Silent Mode\n");
    high_frequency_loop();
    printf("DONE.\n\n");

    setenabled();

    printf("Silent Mode Disabled\n");
    high_frequency_loop();
    printf("DONE.\n");

    return 0;
}
