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

// Customer Construct
Customer::Customer(int paid, bool active,  const Message* messages, int numMessages)
    : paid(paid), active(active), messages(messages), numMessages(numMessages), lastMessageIndex(-1) {
}

bool Customer::isActive() const {
    return active;
}
int Customer::getPay() const {
    return paid;
}

const Message& Customer::getNextMessage(bool isOddMinute) const {
    // Generate a random index to select a message
    int randomIndex;
    do {
        randomIndex = rand() % numMessages;
    // Ensure the same message is not displayed twice in a row (unless we only have one message)
    } while (randomIndex == lastMessageIndex && numMessages > 1); 

    // Update the last displayed message index
    lastMessageIndex = randomIndex; 

     // Check if the message should be displayed based on odd or even minutes
    if ((isOddMinute && messages[randomIndex].oddMinutes()) ||
        (!isOddMinute && messages[randomIndex].evenMinutes()) ||
        (messages[randomIndex].oddMinutes() && messages[randomIndex].evenMinutes())) {
        return messages[randomIndex];
    } else {
        // If the message doesn't match the current minute condition, try again
        return getNextMessage(isOddMinute);
    }

}