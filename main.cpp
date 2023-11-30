#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

// Slightly modified version of previous LCD with different pinouts.
// https://wokwi.com/projects/382661369630538753

// å = \xe5
// ä = \xe4
// ö = \xf6
// Å = \xc5
// Ä = \xc4
// Ö = \xd6

static void infinite_scrolling_faces(LCD& lcd)
{
    uint8_t cgram_buf[64] {
        0x00, 0x00, 0x03, 0x05, 0x0d, 0x06, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 
        0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x08, 0x08, 
        0x00, 0x00, 0x18, 0x14, 0x16, 0x0c, 0x00, 0x00, 
        0x00, 0x08, 0x07, 0x05, 0x06, 0x03, 0x00, 0x00, 
        0x04, 0x00, 0x1f, 0x09, 0x09, 0x1a, 0x0f, 0x00, 
        0x04, 0x00, 0x1f, 0x12, 0x12, 0x0b, 0x1e, 0x00,
        0x00, 0x02, 0x1c, 0x14, 0x0c, 0x18, 0x00, 0x00,
    };
    lcd.set_cgram(cgram_buf);

    lcd.position(1, 0);
    lcd.str_normal("\x08\x09\x0a\x0b");
    lcd.position(1, 1);
    lcd.str_normal("\x0c\x0d\x0e\x0f");

    lcd.position(6, 0);
    lcd.str_normal("\x08\x09\x0a\x0b");
    lcd.position(6, 1);
    lcd.str_normal("\x0c\x0d\x0e\x0f");

    lcd.position(11, 0);
    lcd.str_normal("\x08\x09\x0a\x0b");
    lcd.position(11, 1);
    lcd.str_normal("\x0c\x0d\x0e\x0f");

    // Scrolling in vertical direction, 4x2 bitmap
    while (1) {
        for (int col = 0; col < 4; col++) {
            uint8_t tmp = cgram_buf[(col + 4) * 8 + 7];
            for (int row = 15; row >= 0; row--) {
                unsigned dst_index = (row > 7) ? (col + 4) * 8 + (row - 8) : col * 8 + row;
                unsigned src_index = (row == 8) ? col * 8 + row - 1 : dst_index - 1;
                cgram_buf[dst_index] = cgram_buf[src_index];
            }
            cgram_buf[col * 8 + 0] = tmp;
        }
        lcd.set_cgram(cgram_buf);
        _delay_ms(25);
    }
}

static void self_promotion(LCD& lcd)
{
    uint8_t cgram_buf[64] = {
        0x00, 0x0e, 0x11, 0x04, 0x0a, 0x00, 0x04, 0x00, 
        0x1f, 0x1f, 0x0e, 0x0e, 0x0e, 0x1f, 0x1f, 0x00,
        0x00, 0x00, 0x1f, 0x1f, 0x1b, 0x1f, 0x1f, 0x00,
        0x1f, 0x1f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    lcd.set_cgram(cgram_buf);

    lcd.position(0, 0);
    lcd.str_normal("Synas h\xe4r?  \x08\x09\x0a\x0b");
    lcd.position(0, 1);
    lcd.str_normal("IoT:s reklambyr\xe5");

    _delay_ms(3000);
}

static void default_behaviour(LCD& lcd)
{
    while (1) {
        lcd.clear();
        lcd.position(0, 0);
        lcd.str_scroll("K\xf6p bil hos Harry!");
    
        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("En god bilaff\xe4r!");
        //_delay_ms(1500);
        //lcd.clear();
        lcd.position(0, 1);
        lcd.str_normal("(f\xf6r Harry!)");
        _delay_ms(3000);

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_blink("Hederlige Harrys bilar", 4);
        _delay_ms(1000);

        lcd.clear();
        lcd.str_scroll("K\xf6p paj hos Farmor Anka!");

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("Skynda innan");
        _delay_ms(1500);

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("M\xe5rten har \xe4tit alla pajer");
        _delay_ms(1500);

        lcd.clear();
        lcd.position(0, 0);
        lcd.str_normal("alla pajer!");
        _delay_ms(3000);
    }
}

int main()
{
    LCD lcd;
    lcd.init();
    
    infinite_scrolling_faces(lcd);
    //self_promotion(lcd);
    //default_behaviour(lcd);

    return 0;
}
