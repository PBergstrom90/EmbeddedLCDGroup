#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <time.h>
#include "lcd.h"
#include "customer.h"
#include "message.h"

// Slightly modified version of previous LCD with different pinouts.
// https://wokwi.com/projects/382811813035164673

// å = \xe5
// ä = \xe4
// ö = \xf6
// Å = \xc5
// Ä = \xc4
// Ö = \xd6

// For IoT Bitmap image:
/*static void self_promotion(LCD& lcd)
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
*/

void initCustomerMessages(Customer customers[], const int numCustomers);
void chooseCustomer(LCD lcd);

int main()
{
    LCD lcd;
    lcd.init();

    // Seed the random number generator
    srand((unsigned int)time(NULL));

        const int numCustomers = 5;

    Customer customers[numCustomers];

    // Initialize arrays
    initCustomerMessages(customers, numCustomers);
    
    while (1) {
        
        // DEBUG MSG
        // lcd.clear();
        // lcd.str_normal("Reset loop");
        // _delay_ms(1000);

        // Choose a random number between 0 and 14499
        int randomValue = rand() % 14500;

        // Find the corresponding customer based on the random value
        int selectedCustomerIndex = -1;
        for (int i = 0; i < numCustomers; ++i) {
            if (randomValue >= customers[i].getLowerBound() && randomValue <= customers[i].getUpperBound()) {
                selectedCustomerIndex = i;
                break;
            }
        }

        if (selectedCustomerIndex != -1) {
            Customer& selectedCustomer = customers[selectedCustomerIndex];
            selectedCustomer.getNextMessage(&lcd);
        }
        
        // DEBUG MSG: Reset Watchdog-timer for microcontroller.
        // lcd.clear();
        // lcd.str_normal("Reset watchdog");
        // _delay_ms(1000);
        // wdt_reset();
    }

    return 0;
}

// Function to initialize and return the arrays
void initCustomerMessages(Customer customers[], const int numCustomers) {
    // Arrays of unique messages for each customer

    // Hederlige Harrys Bilar:
    static const Message messagesCustomer1[] = {
        {"K\xf6p bil hos Harry! ", MSGEFF_SCROLL, false, false},
        {"En god bilaff\xe4r\n(f\xf6r Harry! )", MSGEFF_NONE, false, false},
        {"Hederlige Harrys\n     Bilar ", MSGEFF_BLINK, false, false}
    };

    // Farmor Ankas Pajer AB:
    static const Message messagesCustomer2[] = {
        {"K\xf6p paj hos Farmor Anka! ", MSGEFF_SCROLL, false, false},
        {"Skynda innan\nM\xe5rten har \xe4tit alla pajer ", MSGEFF_NONE, false, false}
    };

    // Svarte Petters Svartbyggen:
    static const Message messagesCustomer3[] = {
        {"L\xe5t Petter bygga \xe5t dig ", MSGEFF_SCROLL, true, false},
        {"Bygga svart?\nRing Petter ", MSGEFF_NONE, false, true}
    };

    // Långbens detektivbyrå:
    static const Message messagesCustomer4[] = {
        {"Mysterier? \nRing L\xe5ngben ", MSGEFF_NONE, false, false},
        {"L\xe5ngben fixar\nbiffen ", MSGEFF_NONE, false, false}
    };

    // IoTs reklambyrå:
    static const Message messagesCustomer5[] = {
        {"Synas h\xe4r?  \x08\x09\x0a\x0b\nIoT:s reklambyr\xe5 ", MSGEFF_SCROLL | MSGEFF_BLINK, false, false}
    };

    // Set up customers
    customers[0] = Customer(5000, true, 0, 4999, messagesCustomer1, sizeof(messagesCustomer1) / sizeof(Message));
    customers[1] = Customer(3000, true, 5000, 7999, messagesCustomer2, sizeof(messagesCustomer2) / sizeof(Message));
    customers[2] = Customer(1500, true, 8000, 9499, messagesCustomer3, sizeof(messagesCustomer3) / sizeof(Message));
    customers[3] = Customer(4000, true, 9500, 13499, messagesCustomer4, sizeof(messagesCustomer4) / sizeof(Message));
    customers[4] = Customer(1000, true, 13500, 14499, messagesCustomer5, sizeof(messagesCustomer5) / sizeof(Message));
}