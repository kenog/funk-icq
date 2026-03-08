#include <app/FunkICQ.h>

FunkIcq::FunkIcq(QueueHandle_t uiToAppQueue, QueueHandle_t appToUiQueue, QueueHandle_t appToLayer2Queue, QueueHandle_t layer2ToAppQueue) {
    this->uiToAppQueue = uiToAppQueue;
    this->appToUiQueue = appToUiQueue;
    this->layer2ToAppQueue = layer2ToAppQueue;
    this->appToLayer2Queue = appToLayer2Queue;

    // Wait 1s until everything is available
    vTaskDelay(1000 * portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(funkIcqTask, "FunkICQ", 4096, this, 1, &appTask, 0);
    Serial.println("APP: Task created");
    Serial.flush();
}

void funkIcqTask(void* pvParams) {
    Serial.println("APP: Task started");
    FunkIcq* thisPtr = static_cast<FunkIcq*>(pvParams);
    if(!thisPtr) {
        Serial.println("APP: thisPtr is NULL");
    }
    else {
        Serial.printf("FunkIcq.thisPtr->getQ() = %p\n", thisPtr->uiToAppQueue);
    }
    Serial.flush();


    ChatMessage transmittableMsg;
    ChatMessage receivedMsg;

    QueueHandle_t uiToAppQueue = thisPtr->uiToAppQueue;
    QueueHandle_t appToUiQueue = thisPtr->appToUiQueue;
    QueueHandle_t layer2ToAppQueue = thisPtr->layer2ToAppQueue;
    QueueHandle_t appToLayer2Queue = thisPtr->appToLayer2Queue;

    while(true) {
        if(!thisPtr) {
            Serial.println("APP: thisPtr is NULL");
            continue;
        }

        if(xQueueReceive(uiToAppQueue, (void*) &transmittableMsg, 50 * portTICK_PERIOD_MS)) {
            // We have new user input
            Serial.printf("APP: New user input: '%s'\n", transmittableMsg.message);
            Serial.flush();
            xQueueSend(appToLayer2Queue, (void*) &transmittableMsg, 1000 * portTICK_PERIOD_MS);
        }

        if(xQueueReceive(layer2ToAppQueue, (void*) &receivedMsg, 50 * portTICK_PERIOD_MS)) {
            // We have new incoming message
            Serial.printf("APP: %s: %s'\n", receivedMsg.sender, receivedMsg.message);
            Serial.flush();
            xQueueSend(appToUiQueue, (void*) &receivedMsg, 1000 * portTICK_PERIOD_MS);
        }
    }
}