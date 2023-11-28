#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

// Slightly modified version of previous LCD with different pinouts.
// https://wokwi.com/projects/382661369630538753

int main()
{
    LCD lcd;
    lcd.init();

    while (1) {
        lcd.clear();
        lcd.home();
        lcd.str_normal("Static!");
        _delay_ms(1500);

        lcd.clear();
        lcd.home();
        lcd.str_scroll("A very long scrolling text...");

        lcd.clear();
        lcd.home();
        lcd.str_blink("Blinking!", 5);
    }

    return 0;
}
