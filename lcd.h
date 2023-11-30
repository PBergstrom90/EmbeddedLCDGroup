#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdint.h>

#define LCD_RS (1 << PB0) // PHYS PIN_8
#define LCD_EN (1 << PB1) // PHYS PIN_9

#define LCD_DATA_PORT PORTD // DATA (PHYS PIN_0 -> PIN_7)
#define LCD_CTRL_PORT PORTB // CTRL (PHYS PIN_8 -> PIN_13)

#define LCD_CLEAR      0x01
#define LCD_4_BIT_MODE 0x02

#define LCD_ENTRY_MODE           0x04
#define LCD_EM_SHIFT_CURSOR      0x00
#define LCD_EM_SHIFT_DISPLAY     0x01
#define LCD_EM_DECREMENT         0x00
#define LCD_EM_INCREMENT         0x02

#define LCD_DISPLAY_ONOFF        0x08
#define LCD_CURSOR_NOBLINK       0x00
#define LCD_CURSOR_BLINK         0x01
#define LCD_CURSOR_OFF           0x00
#define LCD_CURSOR_ON            0x02
#define LCD_DISPLAY_OFF          0x00
#define LCD_DISPLAY_ON           0x04

#define LCD_DISPLAY_CURSOR_SHIFT 0x10
#define LCD_SHIFT_LEFT           0x00
#define LCD_SHIFT_RIGHT          0x04
#define LCD_SHIFT_CURSOR         0x00
#define LCD_SHIFT_DISPLAY        0x08

#define LCD_FUNCTION_SET         0x20
#define LCD_FONT5x8              0x00
#define LCD_FONT5x10             0x04
#define LCD_ONE_LINE             0x00
#define LCD_TWO_LINE             0x08
#define LCD_4_BIT                0x00
#define LCD_8_BIT                0x10

#define LCD_CGRAM_SET 0x40
#define LCD_DDRAM_SET 0x80

class LCD {
public:
    LCD()
        : position_x(0)
        , position_y(0)
    { }

    void init();
    void home();
    void position(unsigned x, unsigned y);
    void clear();
    void str_normal(const char* str);
    void str_scroll(const char* str);
    void str_blink(const char* str, unsigned n);
    void set_cgram(uint8_t* buf);
private:
    void send_ch(uint8_t ch);
    void send_command(uint8_t command);
    void pulse_enable();

    int position_x;
    int position_y;
};

#endif
