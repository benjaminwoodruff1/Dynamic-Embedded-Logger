#ifndef TEMPLATE_H
#define TEMPLATE_H

#ifdef __cplusplus
extern "C" {
#endif
    #include <stdio.h>

    extern bool log_enabled;

    void setenabled();

    bool check();

#ifdef __cplusplus
}
#endif

#endif