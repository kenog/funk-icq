#include <Arduino.h>
#include <protocol/Layer2.h>
#include <ChatMessage.h>
#include <Settings.h>


void layer2TaskFunc(void * pvParameters) {
    Layer2 *thisPtr = static_cast<Layer2*>(pvParameters);       // TODO: This approach does not work and the object becomes invalid after a while, change to basic C

    QueueHandle_t appToLayer2Queue = thisPtr->getAppToLayer2QueueHandle();
    QueueHandle_t layer2ToAppQueue = thisPtr->getLayer2ToAppQueueHandle();
    QueueHandle_t layer2ToRadioQueue= thisPtr->getLayer2ToRadioQueueHandle();
    QueueHandle_t radioToLayer2Queue = thisPtr->getRadioToLayer2QueueHandle();

    Layer2::ProtocolState currentState = Layer2::IDLE;

    ChatMessage userInputMessage;
    Frame radioReceivedFrame;

    for(ever) {
        switch(currentState) {
            case Layer2::IDLE:
                if(xQueueReceive(radioToLayer2Queue, (void*) &radioReceivedFrame, 0 * portTICK_PERIOD_MS)) {
                    currentState = Layer2::RX;
                }
                if(xQueueReceive(appToLayer2Queue, (void*) &userInputMessage, 0 * portTICK_PERIOD_MS)) {
                    currentState = Layer2::TX;
                }
                break;
            case Layer2::RX:
                Serial.printf("L2: Receiving Frame...\n");
                // Check for SFD
                // Save all subsequent byts in buffer until EFD received
                // Forward to App
                ChatMessage c;
                strncpy(c.sender, radioReceivedFrame.sender, MAX_USERNAME_LEN);
                memcpy(c.message, radioReceivedFrame.payload, MAX_MSG_LEN);
                xQueueSend(layer2ToAppQueue, (void*) &c, 0 * portTICK_PERIOD_MS);       // We cannot wait here. If the Queue is full, we have to drop the message and hope for a retransmit
                currentState = Layer2::IDLE;
                break;
            case Layer2::TX:
                // Send Frame to Radio and wait for it to signal completion
                Frame f;
                strncpy(f.sender, "keno", MAX_USERNAME_LEN);
                memcpy(f.payload, userInputMessage.message, MAX_MSG_LEN);
                thisPtr->sendFrame(f);
                currentState = Layer2::IDLE;
                break;
        }
        vTaskDelay(10 * portTICK_PERIOD_MS);
    }
}

Layer2::Layer2(QueueHandle_t upperLayerIn, QueueHandle_t upperLayerOut, QueueHandle_t lowerLayerOut, QueueHandle_t lowerLayerIn) {
    appToLayer2Queue = upperLayerIn;
    layer2ToAppQueue = upperLayerOut;
    layer2ToRadioQueue = lowerLayerOut;
    radioToLayer2Queue = lowerLayerIn;

    xTaskCreatePinnedToCore(layer2TaskFunc, "Layer2 Task", 4096, this, 2, &layer2TaskHandle, 0);
}

int Layer2::sendFrame(Frame f) {
    Serial.printf("L2: Sending Frame...\n");
    // Loopback for now
    if(xQueueSend(radioToLayer2Queue, &f, 100 * portTICK_PERIOD_MS) == pdTRUE) {
        return 0;
    }
    return -1;
}