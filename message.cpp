#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "message.h"
#include "customer.h"

Message::Message(const char* text, bool scroll, bool blink, bool evenminutes, bool oddminutes)
    : text(text), scroll(scroll), blink(blink), evenminutes(evenminutes), oddminutes(oddminutes) {
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