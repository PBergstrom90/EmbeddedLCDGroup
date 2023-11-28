#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdint.h>

#define LCD_RS (1 << PB0) // PHYS PIN_8
#define LCD_EN (1 << PB1) // PHYS PIN_9

#define LCD_DATA_PORT PORTD // DATA (PHYS PIN_0 -> PIN_7)
#define LCD_CTRL_PORT PORTB // CTRL (PHYS PIN_8 -> PIN_13)

class LCD {
public:
    void init();
    void home();
    void clear();
    void str_normal(const char* str);
    void str_scroll(const char* str);
    void str_blink(const char* str, unsigned n);
private:
    void send_ch(uint8_t ch);
    void send_command(uint8_t command);
    void pulse_enable();
};

#endif
