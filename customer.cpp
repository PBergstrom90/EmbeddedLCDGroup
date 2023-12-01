#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "customer.h"
#include "message.h"
#include "lcd.h"

// Default Construct
Customer::Customer() : paid(0), active(false), messages(NULL) {
}

Customer::Customer(int paid, bool active,  const Message* messages, int numMessages)
    : paid(paid), active(active), messages(messages), numMessages(numMessages) {
}

void Customer::setActive(bool active) {
    this->active = active;
}
int Customer::getPay() const {
    return paid;
}