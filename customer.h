#ifndef __CUSTOMER_H_
#define __CUSTOMER_H_

#include "message.h"
#include "lcd.h"

class Message;
class Customer {

public:
    Customer();
    Customer(int paid, bool active, const Message* messages, int numMessages);
    void initCustomerMessages(Customer customerList[], const int numCustomers);
    bool isActive() const;
    int getPay() const;
    const Message& getNextMessage(bool isOddMinute) const;

private:
    int paid;
    bool active;
    const Message* messages;
    int numMessages;
    // Add mutable keyword to allow modification in const functions
    mutable int lastMessageIndex;
};

#endif // __CUSTOMER_H_