#pragma once

#include <Arduino.h>
#include <ChatMessage.h>

void funkIcqTask(void* pvParams);

class FunkIcq {
    public:
        FunkIcq(QueueHandle_t uiToAppQueue, QueueHandle_t appToUiQueue, QueueHandle_t appToLayer2Queue, QueueHandle_t layer2ToAppQueue);
        void handleIncomingMessage(ChatMessage msg);
        static void handleUserInput(String input);

        QueueHandle_t uiToAppQueue;
        QueueHandle_t appToUiQueue;
        QueueHandle_t layer2ToAppQueue;
        QueueHandle_t appToLayer2Queue;
    private:
        TaskHandle_t appTask;
};