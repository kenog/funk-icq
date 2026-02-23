#pragma once

#include <Arduino.h>

class ChatMessage {
    public:
        String sender;
        String message;
        unsigned int receptionTime;
};