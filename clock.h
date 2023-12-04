#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

void init_clock();

uint32_t elapsed_seconds();
bool is_odd_minute();

#endif
