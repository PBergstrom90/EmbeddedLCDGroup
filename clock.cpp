#include "clock.h"
#include <avr/interrupt.h>

volatile uint32_t elapsed_s = 0;

ISR(TIMER1_OVF_vect)
{
    elapsed_s++;
    TCNT1 = 65535 - (F_CPU / 1024);
}

void init_clock()
{
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 65535 - (F_CPU / 1024);

    TCCR1A = 0;
    TIMSK1 = (1 << TOIE1);

    sei();
}

uint32_t elapsed_seconds()
{
    return elapsed_s;
}

bool is_odd_minute()
{
    return (elapsed_s / 60) % 2;
}
