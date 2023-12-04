#ifndef __CUSTOMER_H_
#define __CUSTOMER_H_

#include "message.h"
#include "lcd.h"

class Message;

class Customer {

public:
    Customer();
    Customer(int paid, bool active, const Message* messages, int numMessages);
    void setActive(bool active);
    void initCustomerMessages(Customer customerList[], const int numCustomers);
    int getPay() const;
    const Message& getNextMessage() const;

private:
    int paid;
    bool active;
    const Message* messages;
    int numMessages;
    mutable int lastMessageIndex; // Add mutable keyword to allow modification in const functions
};

#endif // __CUSTOMER_H_