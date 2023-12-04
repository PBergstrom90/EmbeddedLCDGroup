#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "message.h"
#include "customer.h"

Message::Message(const char* text, unsigned flags, bool evenminutes, bool oddminutes)
    : text(text), scroll(flags & MSGEFF_SCROLL), blink(flags & MSGEFF_BLINK), evenminutes(evenminutes), oddminutes(oddminutes) {
}

const char* Message::getText() const {
    return text;
}

bool Message::shouldScroll() const {
    return scroll;
}

bool Message::shouldBlink() const {
    return blink;
}