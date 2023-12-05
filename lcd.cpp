#include "lcd.h"
#include <util/delay.h>

void LCD::init()
{
    DDRD |= 0xf0;
    DDRB |= LCD_RS | LCD_EN;
    _delay_ms(50);

    send_command(LCD_4_BIT_MODE);
    send_command(LCD_FUNCTION_SET  | (LCD_FONT5x8         | LCD_TWO_LINE     | LCD_4_BIT));
    send_command(LCD_DISPLAY_ONOFF | (LCD_CURSOR_NOBLINK  | LCD_CURSOR_OFF   | LCD_DISPLAY_ON));
    send_command(LCD_ENTRY_MODE    | (LCD_EM_SHIFT_CURSOR | LCD_EM_INCREMENT));
    clear();
    _delay_ms(2);
}

void LCD::home()
{
    send_command(LCD_HOME);
    _delay_ms(2);
}

void LCD::position(unsigned x, unsigned y)
{
    x = (x < 0) ? x = 0 : (x > 15) ? x = 15 : x;
    y = (y < 0) ? y = 0 : (y >  1) ? y =  1 : y;
    send_command(LCD_DDRAM_SET | ((0x40 * y) + x));
    position_x = x;
    position_x = y;
}

void LCD::clear()
{
    send_command(LCD_CLEAR);
    _delay_ms(2);
}

void LCD::str_normal(const char *str)
{
    while (*str) {
        // A really simple UTF-8 handler that only handles the first two bytes
        // of a sequence, since that's all we need for nordic characters.
        uint32_t code_point = 0;
        if ((*str & 0x80) == 0) {
            code_point = *str;
            str++;
        } else if ((*str & 0xe0) == 0xc0) {
            code_point = (*str & 0x1f) << 6;
            str++;

            code_point |= *str & 0x3f;
            str++;
        }
        
        if (code_point == '\n') {
            position(0, position_y + 1);
        } else {
            uint8_t ch;
            switch (code_point) {
                case 0x00e5: ch = (variant == LCDVariant::A00) ? '\x04' : '\xe5'; break; // å
                case 0x00e4: ch = (variant == LCDVariant::A00) ? '\xe1' : '\xe4'; break; // ä
                case 0x00f6: ch = (variant == LCDVariant::A00) ? '\xef' : '\xf6'; break; // ö
                case 0x00c5: ch = (variant == LCDVariant::A00) ? '\x05' : '\xc5'; break; // Å
                case 0x00c4: ch = (variant == LCDVariant::A00) ? '\x06' : '\xc4'; break; // Ä
                case 0x00d6: ch = (variant == LCDVariant::A00) ? '\x07' : '\xd6'; break; // Ö
                default: 
                    ch = code_point;
                    break;
            }

            send_ch(ch);
            position_x++;
        }
    }
}

void LCD::str_scroll(const char *str)
{
    int x = 15;
    do {
        clear();
        position(x, 0);
        for (unsigned i = 0; i < (16 - x); i++) {
            send_ch(str[i]);
        }
        _delay_ms(150);
        x--;
    } while (x > 0);

    do {
        clear();
        position(0, 0);
        str_normal(str++);
        _delay_ms(150);
    } while (*str);
}

void LCD::str_blink(const char *str, unsigned n)
{
    for (unsigned i = 0; i < n; i++) {
        clear();
        _delay_ms(400);
        str_normal(str);
        _delay_ms(400);
    }
}

void LCD::set_cgram(uint8_t *buf)
{
    for (int ch_index = 0; ch_index < 8; ch_index++) {
        send_command(LCD_CGRAM_SET | (ch_index << 3));
        for (int row = 0; row < 8; row++)
            send_ch(buf[ch_index * 8 + row]);
    }
}

void LCD::send_ch(uint8_t ch)
{
    // High nibble
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | (ch & 0xf0);
    LCD_CTRL_PORT |= LCD_RS;
    pulse_enable();
    _delay_us(100);

    // Low nibble
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | ((ch << 4) & 0xf0);
    LCD_CTRL_PORT |= LCD_RS;
    pulse_enable();
    _delay_us(100);
}

void LCD::send_command(uint8_t command)
{
    // High nibble
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | (command & 0xf0);
    LCD_CTRL_PORT &= ~LCD_RS;
    pulse_enable();
    _delay_us(100);

    // Low nibble
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f) | ((command << 4) & 0xf0);
    LCD_CTRL_PORT &= ~LCD_RS;
    pulse_enable();
    _delay_us(100);

    _delay_us(50);
}

void LCD::pulse_enable()
{
    LCD_CTRL_PORT |= LCD_EN;
    _delay_us(1);
    LCD_CTRL_PORT &= ~LCD_EN;
}
