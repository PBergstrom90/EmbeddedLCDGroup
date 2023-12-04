#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>
#include "customer.h"

enum MessageEffects {
    MSGEFF_NONE   = 0,
    MSGEFF_SCROLL = (1 << 0),
    MSGEFF_BLINK  = (1 << 1),
};

class Message {
public:
    Message(const char* text, unsigned flags, bool evenminutes, bool oddminutes);
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