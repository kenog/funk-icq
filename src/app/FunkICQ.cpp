#include <app/FunkICQ.h>

FunkIcq::FunkIcq(QueueHandle_t uiToAppQueue, QueueHandle_t appToUiQueue, QueueHandle_t appToLayer2Queue, QueueHandle_t layer2ToAppQueue) {
    this->uiToAppQueue = uiToAppQueue;
    this->appToUiQueue = appToUiQueue;
    this->layer2ToAppQueue = layer2ToAppQueue;
    this->appToLayer2Queue = appToLayer2Queue;

    // Wait 1s until everything is available
    vTaskDelay(1000 * portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(funkIcqTask, "FunkICQ", 4096, this, 1, &appTask, 0);
    D_println("APP: Task created");
    D_flush();
}

void funkIcqTask(void* pvParams) {
    D_println("APP: Task started");
    FunkIcq* thisPtr = static_cast<FunkIcq*>(pvParams);
    if(!thisPtr) {
        D_println("APP: thisPtr is NULL");
    }
    else {
        D_printf("FunkIcq.thisPtr->getQ() = %p\n", thisPtr->uiToAppQueue);
    }
    D_flush();


    ChatMessage transmittableMsg;
    ChatMessage receivedMsg;

    QueueHandle_t uiToAppQueue = thisPtr->uiToAppQueue;
    QueueHandle_t appToUiQueue = thisPtr->appToUiQueue;
    QueueHandle_t layer2ToAppQueue = thisPtr->layer2ToAppQueue;
    QueueHandle_t appToLayer2Queue = thisPtr->appToLayer2Queue;

    while(true) {
        if(!thisPtr) {
            D_println("APP: thisPtr is NULL");
            continue;
        }

        if(xQueueReceive(uiToAppQueue, (void*) &transmittableMsg, 50 * portTICK_PERIOD_MS)) {
            // We have new user input
            D_printf("APP: New user input: '%s'\n", transmittableMsg.message);
            xQueueSend(appToLayer2Queue, (void*) &transmittableMsg, 1000 * portTICK_PERIOD_MS);
        }
        if(xQueueReceive(layer2ToAppQueue, (void*) &receivedMsg, 50 * portTICK_PERIOD_MS)) {
            // We have new incoming message
            D_printf("APP: %s: %s'\n", receivedMsg.sender, receivedMsg.message);
            xQueueSend(appToUiQueue, (void*) &receivedMsg, 1000 * portTICK_PERIOD_MS);
        }
    }
}