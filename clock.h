#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <stdint.h>

void init_clock();
uint32_t elapsed_seconds();
bool is_odd_minute();

#endif // __CLOCK_H_
