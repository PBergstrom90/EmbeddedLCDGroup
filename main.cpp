#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "customer.h"
#include "message.h"
#include "clock.h"

// Seconds to show each customer, excluding delaytimes.
#define SECONDS_PER_CUSTOMER 18

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

void initCustomerMessages(Customer customerList[]);
void displayNextMessage(LCD* lcd, const Customer& customer, bool isOddMinute);
int customerRng(Customer* customerList, uint8_t numCustomers);

int main()
{
    
    init_clock();
    LCD lcd;
    lcd.init();

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    const uint8_t numCustomers = 5;
    Customer customerList[numCustomers];

    // Initialize arrays
    initCustomerMessages(customerList);

    uint32_t systemStart = elapsed_seconds();
    uint32_t lastUpdate = systemStart;
    char systemStartString[20];

    while (1) {
        // Update the system start time if a minute has passed
        if (elapsed_seconds() - lastUpdate >= 1) {
            systemStart = elapsed_seconds();
            lastUpdate = systemStart;
        }
        bool isOddMinute  = is_odd_minute();
        
        // DEBUG MSG: Show seconds since system start.
        sprintf(systemStartString, "Seconds: %ld" , systemStart);
        lcd.clear();
        lcd.str_normal(systemStartString);
        _delay_ms(1000);
        lcd.clear();


        int customerIndex = customerRng(customerList, numCustomers);
        displayNextMessage(&lcd, customerList[customerIndex], isOddMinute);
    
    }

    return 0;
}

void displayNextMessage(LCD* lcd, const Customer& customer, bool isOddMinute) {
    // Access and display the selected message on the LCD
    const Message& selectedMessage = customer.getNextMessage(isOddMinute);
    bool loop = true;
        uint32_t start_s = elapsed_seconds();
        while (loop){
            
            if (selectedMessage.shouldScroll()) {
            lcd->str_scroll(selectedMessage.getText());
            _delay_ms(1000);
            } else if (selectedMessage.shouldBlink()) {
            lcd->clear();
            lcd->str_blink(selectedMessage.getText(), 4);
            _delay_ms(1000);
            } else {
            lcd->clear();
            lcd->str_normal(selectedMessage.getText());
            _delay_ms(2000);
            }
        if (elapsed_seconds() - start_s >= SECONDS_PER_CUSTOMER) {
            lcd->clear();
            loop = false;  // Exit the loop
            }
        }
 }

int customerRng(Customer* customers, uint8_t numCustomers)
{
    // Initialize the previous customer index.
    static int prevCustomerIndex = -1;
    
    while (true) {
        // Calculate total amount of cash for active customers.
        int totalCash = 0;
        for (uint8_t i = 0; i < numCustomers; i++) {
            if (customers[i].isActive()) {
                totalCash += customers[i].getPay();
            }
        }

        // If there are no active customers, try again.
        if (totalCash == 0) {
            continue;
        }

        unsigned int randomValue = rand() % totalCash;
        unsigned int lowerBound = 0;
        int luckyCustomerIndex = -1;

        for (uint8_t i = 0; i < numCustomers; i++) {
            if (customers[i].isActive()) {
                if (randomValue >= lowerBound && randomValue < lowerBound + customers[i].getPay()) {
                    // We found our winner
                    luckyCustomerIndex = i;
                    break;
                }
                // Increase our lower bound for the next customer.
                lowerBound += customers[i].getPay();
            }
        }

        // If the same customer is chosen again, continue to the next iteration.
        if (luckyCustomerIndex == prevCustomerIndex) {
            continue;
        }
        prevCustomerIndex = luckyCustomerIndex;
        return luckyCustomerIndex;
    }
}

// Function to initialize and return the customerarrays.
void initCustomerMessages(Customer customerList[]) {

    // Hederlige Harrys Bilar:
    static const Message messagesCustomer1[] = {
        {"K\xf6p bil hos Harry! ", MSGEFF_SCROLL, true, true},
        {"En god bilaff\xe4r\n(f\xf6r Harry!)", MSGEFF_NONE, true, true},
        {"Hederlige Harrys\n     Bilar ", MSGEFF_BLINK, true, true}
    };

    // Farmor Ankas Pajer AB:
    static const Message messagesCustomer2[] = {
        {"K\xf6p paj hos Farmor Anka! ", MSGEFF_SCROLL, true, true},
        {"Skynda innan\nM\xe5rten har \xe4tit alla pajer ", MSGEFF_NONE, true, true}
    };

    // Svarte Petters Svartbyggen:
    static const Message messagesCustomer3[] = {
        {"L\xe5t Petter bygga \xe5t dig ", MSGEFF_SCROLL, true, false},
        {"Bygga svart?\nRing Petter ", MSGEFF_NONE, false, true}
    };

    // Långbens detektivbyrå:
    static const Message messagesCustomer4[] = {
        {"Mysterier? \nRing L\xe5ngben ", MSGEFF_NONE, true, true},
        {"L\xe5ngben fixar\nbiffen ", MSGEFF_NONE, true, true}
    };

    // IoTs reklambyrå:
    static const Message messagesCustomer5[] = {
        {"Synas h\xe4r?  \x08\x09\x0a\x0b\nIoT:s reklambyr\xe5 ", MSGEFF_NONE, true, true}
    };

    // Set up customers
    customerList[0] = Customer(5000, true, messagesCustomer1, sizeof(messagesCustomer1) / sizeof(Message));
    customerList[1] = Customer(3000, true, messagesCustomer2, sizeof(messagesCustomer2) / sizeof(Message));
    customerList[2] = Customer(1500, true, messagesCustomer3, sizeof(messagesCustomer3) / sizeof(Message));
    customerList[3] = Customer(4000, true, messagesCustomer4, sizeof(messagesCustomer4) / sizeof(Message));
    customerList[4] = Customer(1000, true, messagesCustomer5, sizeof(messagesCustomer5) / sizeof(Message));
}