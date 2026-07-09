#include "template.h"

extern "C" {
    bool log_enabled = false;

    void setenabled(){
        log_enabled = true;
    }

    bool check(){
        printf("checking\n");
        return log_enabled;
    }
}