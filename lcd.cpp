#include "lcd.h"
#include <util/delay.h>

void LCD::init()
{
    DDRD |= 0xf0;
    DDRB |= LCD_RS | LCD_EN;
    _delay_ms(50);

    send_command(0x02); // Initialize in 4-bit mode
    send_command(0x28); // 2 lines, 5x8 matrix
    send_command(0x0c); // Display on, cursor off
    send_command(0x06); // Increment cursor
    send_command(0x01); // Clear display
    _delay_ms(2);
}

void LCD::home()
{
    send_command(0x80);
}

void LCD::position(unsigned x, unsigned y)
{
    send_command(0x80 | ((0x40 * y) + x));
}

void LCD::clear()
{
    send_command(0x01);
}

void LCD::str_normal(const char *str)
{
    while (*str)
        send_ch(*str++);
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
}

void LCD::pulse_enable()
{
    LCD_CTRL_PORT |= LCD_EN;
    _delay_us(1);
    LCD_CTRL_PORT &= ~LCD_EN;
}

