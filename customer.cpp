#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "customer.h"
#include "message.h"
#include "lcd.h"

// Default Construct
Customer::Customer() : paid(0), active(false), messages(NULL), numMessages(0), lastMessageIndex(-1) {
}

Customer::Customer(int paid, bool active,  const Message* messages, int numMessages)
    : paid(paid), active(active), messages(messages), numMessages(numMessages), lastMessageIndex(-1) {
}

void Customer::setActive(bool active) {
    this->active = active;
}
int Customer::getPay() const {
    return paid;
}

const Message& Customer::getNextMessage() const {
    // Generate a random index to select a message
    int randomIndex;
    do {
        randomIndex = rand() % numMessages;
    } while (randomIndex == lastMessageIndex);  // Ensure the same message is not displayed twice in a row

    lastMessageIndex = randomIndex;  // Update the last displayed message index

    return messages[randomIndex];
}