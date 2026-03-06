#pragma once

#include <Arduino.h>
#include <Settings.h>

class ChatMessage {
    public:
        char sender[MAX_USERNAME_LEN];
        char message[MAX_MSG_LEN];
        unsigned int receptionTime;
};