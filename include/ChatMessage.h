#pragma once

#include <Arduino.h>

class ChatMessage {
    public:
        std::string sender;
        uint8_t *data;
        unsigned int receptionTime;
};