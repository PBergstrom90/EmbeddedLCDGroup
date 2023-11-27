#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"
#include "bitMacros.h"

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
    lcd.GoTo(0,0);
    lcd.WriteText("Hej Hej ");

    char text[20];

     while(1){
         lcd.Clear();
         lcd.GoTo(0,0);
         lcd.WriteText("Halloj");
         _delay_ms(1000);
     }
    return 0;
}
