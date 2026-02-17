#include <radio/Dra818.h>
#include <protocol/FrameCodec.h>

Dra818::Dra818(QueueHandle_t fromUpperLayer, QueueHandle_t toUpperLayer) {
    Serial.println("Initialising DRA818 Radio...");
    powerMode = LOW_POWER;
    fromUpperLayerQueue = fromUpperLayer;
    toUpperLayerQueue = toUpperLayer;

    // TODO: Set config via UART

    xTaskCreate(dra818TxTask, "Dra818Task", 4096, this, 1, NULL);
}

void dra818TxTask(void* pvParams) {
    Frame frame;
    Dra818* thisPtr = static_cast<Dra818*>(pvParams);

    uint8_t buf[128];
    while(true) {
        if(xQueueReceive(thisPtr->getFromUpperLayerQueueHandle(), &frame, 1000 * portTICK_PERIOD_MS)) {
            thisPtr->sendBytes(frame.payload, frame.length);
        }
    }
}