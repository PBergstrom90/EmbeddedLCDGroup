#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <time.h>
#include "lcd.h"
#include "customer.h"
#include "message.h"

// Slightly modified version of previous LCD with different pinouts.
// https://wokwi.com/projects/382661369630538753

// å = \xe5
// ä = \xe4
// ö = \xf6
// Å = \xc5
// Ä = \xc4
// Ö = \xd6

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
    
    int prevCustomerIndex = 0;

    while (1) {
        // Choose a random customer based on the likelihood defined by lowerBound and upperBound
        int totalRange = 0;
        for (int i = 0; i < numCustomers; ++i) {
            totalRange += customers[i].getUpperBound() - customers[i].getLowerBound() + 1;
        }
        int randomValue = rand() % totalRange;
        int selectedCustomerIndex = -1;
        for (int i = 0; i < numCustomers; ++i) {
            randomValue -= customers[i].getUpperBound() - customers[i].getLowerBound() + 1;

            if (randomValue <= 0) {
                selectedCustomerIndex = i;
                break;
            }
        }

        // Ensure the newly selected customer is different from the previous one
        while (selectedCustomerIndex == prevCustomerIndex) {
            randomValue = rand() % totalRange;
            selectedCustomerIndex = -1;
            for (int i = 0; i < numCustomers; ++i) {
                randomValue -= customers[i].getUpperBound() - customers[i].getLowerBound() + 1;
                if (randomValue <= 0) {
                    selectedCustomerIndex = i;
                    break;
                }
            }
        }
        prevCustomerIndex = selectedCustomerIndex;  // Update the previous customer index
        Customer& selectedCustomer = customers[selectedCustomerIndex];

        // Display the next message for the selected customer on LCD
        selectedCustomer.getNextMessage(&lcd);
    }

    return 0;
}

// Function to initialize and return the arrays
void initCustomerMessages(Customer customers[], const int numCustomers) {
    // Arrays of unique messages for each customer

    // Hederlige Harrys Bilar:
    static const Message messagesCustomer1[] = {
        {"K\xf6p bil hos Harry! ", true, false, false, false},
        {"En god bilaff\xe4r\n(f\xf6r Harry! )", false, false, false, false},
        {"Hederlige Harrys\n     Bilar ", false, true, false, false}
    };

    // Farmor Ankas Pajer AB:
    static const Message messagesCustomer2[] = {
        {"K\xf6p paj hos Farmor Anka! ", true, false, false, false},
        {"Skynda innan\nM\xe5rten har \xe4tit alla pajer ", false, false, false, false}
    };

    // Svarte Petters Svartbyggen:
    static const Message messagesCustomer3[] = {
        {"L\xe5t Petter bygga \xe5t dig ", true, false, true, false},
        {"Bygga svart?\nRing Petter ", false, false, false, true}
    };

    // Långbens detektivbyrå:
    static const Message messagesCustomer4[] = {
        {"Mysterier? \nRing L\xe5ngben ", false, false, false, false},
        {"L\xe5ngben fixar\nbiffen ", false, false, false, false}
    };

    // IoTs reklambyrå:
    static const Message messagesCustomer5[] = {
        {"Synas h\xe4r?  \x08\x09\x0a\x0b\nIoT:s reklambyr\xe5 ", false, false, false, false}
    };

    // Set up customers
    customers[0] = Customer(5000, true, 0, 4999, messagesCustomer1, sizeof(messagesCustomer1) / sizeof(Message));
    customers[1] = Customer(3000, true, 5000, 7999, messagesCustomer2, sizeof(messagesCustomer2) / sizeof(Message));
    customers[2] = Customer(1500, true, 8000, 9499, messagesCustomer3, sizeof(messagesCustomer3) / sizeof(Message));
    customers[3] = Customer(4000, true, 9500, 13499, messagesCustomer4, sizeof(messagesCustomer4) / sizeof(Message));
    customers[4] = Customer(1000, true, 13500, 14499, messagesCustomer5, sizeof(messagesCustomer5) / sizeof(Message));
}