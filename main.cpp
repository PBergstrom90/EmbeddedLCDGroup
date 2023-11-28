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
        lcd.position(0, 0);
        lcd.str_scroll("Kop bil hos Harry!");
    
        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("En god bilaffar!");
        _delay_ms(1500);
        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("(for Harry!)");
        _delay_ms(3000);

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_blink("Hederlige Harrys bilar", 4);
        _delay_ms(1000);

        lcd.clear();
        lcd.str_scroll("Kop paj hos Farmor Anka!");

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("Skynda innan");
        _delay_ms(1500);

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("Marten har atit alla pajer");
        _delay_ms(1500);

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("alla pajer!");
        _delay_ms(3000);
    }

    return 0;
}
