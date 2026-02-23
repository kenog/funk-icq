#pragma once

#include <Arduino.h>
#include <ChatMessage.h>

void funkIcqTask(void* pvParams);

class FunkIcq {
    public:
        FunkIcq(QueueHandle_t uiToAppQueue);
        void handleIncomingMessage(ChatMessage msg);
        static void handleUserInput(String input);
        QueueHandle_t getUiToAppQueue() {
            return uiToAppQueue;
        }
    private:
        TaskHandle_t appTask;
        QueueHandle_t uiToAppQueue;
};