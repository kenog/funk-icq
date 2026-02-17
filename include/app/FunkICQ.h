#pragma once

#include <Arduino.h>
#include <ChatMessage.h>

void funkIcqTask(void* pvParams);

class FunkIcq {
    public:
        FunkIcq();
        void handleIncomingMessage(const ChatMessage& msg);
        void handleUserInput(const String& input);
};