#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "customer.h"
#include "message.h"
#include "lcd.h"

Customer::Customer() : paid(0), active(false), lowerBound(0), upperBound(0), messages(NULL), numMessages(0), lastMessageIndex(-1) {
}

Customer::Customer(int paid, bool active, int lowerBound, int upperBound, const Message* messages, int numMessages)
    : paid(paid), active(active), lowerBound(lowerBound), upperBound(upperBound), messages(messages), numMessages(numMessages), lastMessageIndex(-1) {
}

void Customer::setActive(bool active) {
    this->active = active;
}
int Customer::getLowerBound() const {
    return lowerBound;
}

int Customer::getUpperBound() const {
    return upperBound;
}
void Customer::getNextMessage(LCD* lcd) {
    // Generate a random index to select a message
    int randomIndex;
    do {
        randomIndex = rand() % numMessages;
    } while (randomIndex == lastMessageIndex);  // Ensure the same message is not displayed twice in a row

    lastMessageIndex = randomIndex;  // Update the last displayed message index

    // Access and display the selected message on the LCD
    const Message& selectedMessage = messages[randomIndex];
    lcd->clear();
    if (selectedMessage.shouldScroll()) {
        lcd->str_scroll(selectedMessage.getText());
        _delay_ms(1000);
    } else if (selectedMessage.shouldBlink()) {
        lcd->str_blink(selectedMessage.getText(), 4);
        _delay_ms(1000);
    } else {
        lcd->str_normal(selectedMessage.getText());
        _delay_ms(2000);
    }
}