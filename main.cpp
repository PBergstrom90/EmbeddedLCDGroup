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
#define SECONDS_PER_CUSTOMER 19

// Slightly modified version of previous LCD with different pinouts.
// https://wokwi.com/projects/382811813035164673

// å = \xe5
// ä = \xe4
// ö = \xf6
// Å = \xc5
// Ä = \xc4
// Ö = \xd6

// For IoT bitmap image:
#define CC_0 "\x08" // We can't use \x00 since that's also the null-character
#define CC_1 "\x01"
#define CC_2 "\x02"
#define CC_3 "\x03"
#define CC_4 "\x04"
#define CC_5 "\x05"
#define CC_6 "\x06"
#define CC_7 "\x07"

// Initialize customerarray.
void initCustomerMessages(Customer customerList[]);

// Show message on LCD
void displayNextMessage(LCD* lcd, const Customer& customer, bool isOddMinute);

// Choose random customer.
int customerRng(Customer* customerList, uint8_t numCustomers);

// DEBUG-function. To display seconds passed.
void timerSeconds(LCD* lcd, uint32_t systemStart, uint32_t lastUpdate, char systemStartString[20]);

int main()
{
    init_clock();
    LCD lcd(LCDVariant::A02);
    lcd.init();
    srand((unsigned int)time(NULL));
    
    // For custom characters used in IoT:s reklambyrå ad
    uint8_t cgram_buf[64] = {
        0x00, 0x0e, 0x11, 0x04, 0x0a, 0x00, 0x04, 0x00, 
        0x1f, 0x1f, 0x0e, 0x0e, 0x0e, 0x1f, 0x1f, 0x00,
        0x00, 0x00, 0x1f, 0x1f, 0x1b, 0x1f, 0x1f, 0x00,
        0x1f, 0x1f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x00,
        0x04, 0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f, 0x00,
        0x04, 0x00, 0x0e, 0x11, 0x1f, 0x11, 0x11, 0x00,
        0x0a, 0x00, 0x0e, 0x11, 0x1f, 0x11, 0x11, 0x00,
        0x0a, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00,
    };
    lcd.set_cgram(cgram_buf);

    const uint8_t numCustomers = 5;
    Customer customerList[numCustomers];
    initCustomerMessages(customerList);

    uint32_t systemStart = elapsed_seconds();
    uint32_t lastUpdate = systemStart;
    char systemStartString[20];

    // Main programloop:
    while (1) {
        bool isOddMinute  = is_odd_minute();
        
        // For Debug:
        // timerSeconds(&lcd, systemStart, lastUpdate, systemStartString);

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
    
    int activeCustomers = 0;
    while (true) {
        // Calculate total amount of cash for active customers.
        int totalCash = 0;
        for (uint8_t i = 0; i < numCustomers; i++) {
            if (customers[i].isActive()) {
                totalCash += customers[i].getPay();
                activeCustomers++;
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
        if (luckyCustomerIndex == prevCustomerIndex && activeCustomers > 1) {
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
        {"Köp bil hos Harry! ", MSGEFF_SCROLL, true, true},
        {"En god bilaffär\n(för Harry!)", MSGEFF_NONE, true, true},
        {"Hederlige Harrys\n     Bilar ", MSGEFF_BLINK, true, true}
    };

    // Farmor Ankas Pajer AB:
    static const Message messagesCustomer2[] = {
        {"Köp paj hos Farmor Anka! ", MSGEFF_SCROLL, true, true},
        {"Skynda innan\nMårten har ätit alla pajer ", MSGEFF_NONE, true, true}
    };

    // Svarte Petters Svartbyggen:
    static const Message messagesCustomer3[] = {
        {"Låt Petter bygga åt dig ", MSGEFF_SCROLL, true, false},
        {"Bygga svart?\nRing Petter ", MSGEFF_NONE, false, true}
    };

    // Långbens detektivbyrå:
    static const Message messagesCustomer4[] = {
        {"Mysterier? \nRing Långben ", MSGEFF_NONE, true, true},
        {"Långben fixar\nbiffen ", MSGEFF_NONE, true, true}
    };

    // IoTs reklambyrå:
    static const Message messagesCustomer5[] = {
        {"Synas här?  " CC_0 CC_1 CC_2 CC_3 "\nIoT:s reklambyrå", MSGEFF_NONE, true, true}
    };
    // Set up customers
    customerList[0] = Customer(5000, true, messagesCustomer1, sizeof(messagesCustomer1) / sizeof(Message));
    customerList[1] = Customer(3000, true, messagesCustomer2, sizeof(messagesCustomer2) / sizeof(Message));
    customerList[2] = Customer(1500, true, messagesCustomer3, sizeof(messagesCustomer3) / sizeof(Message));
    customerList[3] = Customer(4000, true, messagesCustomer4, sizeof(messagesCustomer4) / sizeof(Message));
    customerList[4] = Customer(1000, true, messagesCustomer5, sizeof(messagesCustomer5) / sizeof(Message));
}

// DEBUG-function, to display seconds elapsed.
void timerSeconds(LCD* lcd, uint32_t systemStart, uint32_t lastUpdate, char systemStartString[20]) {
        // Update the system start time if a minute has passed.
        if (elapsed_seconds() - lastUpdate >= 1) {
            systemStart = elapsed_seconds();
            lastUpdate = systemStart;
        }
        // Show Seconds on LCD.
        sprintf(systemStartString, "Seconds: %ld" , systemStart);
        lcd->clear();
        lcd->str_normal(systemStartString);
        _delay_ms(1000);
        lcd->clear();
}