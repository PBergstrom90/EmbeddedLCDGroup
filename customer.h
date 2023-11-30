#ifndef __CUSTOMER_H_
#define __CUSTOMER_H_

#include "message.h"
#include "lcd.h"

class Message;

class Customer {

public:
    Customer();
    Customer(int paid, bool active, int lowerBound, int upperBound, const Message* messages, int numMessages);
    void setActive(bool active);
    void getNextMessage(LCD* lcd);
    void initCustomerMessages(Customer customers[], const int numCustomers);

private:
    int paid;
    bool active;
    int lowerBound;
    int upperBound;
    const Message* messages;
    int numMessages;
    int lastMessageIndex;
};

#endif // __CUSTOMER_H_