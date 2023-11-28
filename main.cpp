#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"
#include "bitMacros.h"
#include "customer.h"
#include "message.h"

//1. Tips "lottery"
    // Company A buys 5000 lottery tickets 0-4999
    // Company B buys 3000 lottery tickets 5000-7999
    // Company C buys 1000 lottery tickets 8000-8999

    //0 - 8999

// No Buttons in this project, only LCD.
// https://wokwi.com/projects/382552146221115393

// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
#define LED_PIN 2
#define BUTTON_PIN 1
#define BUTTON_IS_CLICKED(PINB,BUTTON_PIN) !BIT_CHECK(PINB,BUTTON_PIN)

int main(void){

    HD44780 lcd;
    lcd.Initalize();

     while(1){

        // Hederlige Harrys Bilar:
        
        lcd.Clear();
        lcd.GoTo(0,0);
        const char scrollingText01[] = "Kop bil hos Harry! ";
        lcd.ScrollText(scrollingText01);

        lcd.Clear();
        lcd.GoTo(0,0);
        lcd.WriteText("En god bilaffar ");
        _delay_ms(1500);
        lcd.Clear();
        lcd.GoTo(0,0);
        lcd.WriteText("(for Harry!) ");
        _delay_ms(3000);
        
        lcd.Clear();
        lcd.GoTo(0,0);
        const char blinkingText[] = "Hederlige Harrys Bilar ";
        lcd.BlinkText(blinkingText, 4);  // Blink 4 times
        lcd.Clear();
        lcd.GoTo(0,0);
        _delay_ms(1000);

        // Farmor Anka: 
        const char scrollingText02[] = "Kop paj hos Farmor Anka! ";
        lcd.ScrollText(scrollingText02);

        lcd.Clear();
        lcd.GoTo(0,0);
        lcd.WriteText("Skynda innan ");
        _delay_ms(1500);
        lcd.Clear();
        lcd.GoTo(0,0);
        lcd.WriteText("Marten har atit alla pajer ");
         _delay_ms(1500);
        lcd.Clear();
        lcd.GoTo(0,0);
        lcd.WriteText("alla pajer! ");
        _delay_ms(3000);

     }
    return 0;
}
