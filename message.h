#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>
#include "customer.h"

class Message {
public:
    Message(const char* text, bool scroll, bool blink, bool evenminutes, bool oddminutes);
    const char* getText() const;
    bool shouldScroll() const;
    bool shouldBlink() const;
private:
    const char* text;
    bool scroll;
    bool blink;
    bool evenminutes;
    bool oddminutes;
};

#endif // MESSAGE_H_