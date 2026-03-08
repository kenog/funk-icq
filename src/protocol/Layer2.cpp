#include <Arduino.h>
#include <protocol/Layer2.h>
#include <ChatMessage.h>
#include <Settings.h>

int serialise(const icqFrame_t& in, uint8_t* buf) {
    D_printf("s0");
    D_flush();

    uint8_t totalLength = sizeof(icqFrame_t) + in.payloadLength + in.senderNameLength;  // TODO: pointers to name & payload buffers are also one byte. Is this okay for SFD + EFD?
    D_printf("s1, len = %d\n", totalLength);
    D_flush();

    void* p = malloc(1);
    D_printf("malloc %d -> %p\n", totalLength, p);
    D_flush();
    delay(1000);
    //buf = (uint8_t*) malloc(1);
    //buf = (uint8_t*) malloc(totalLength);
    D_printf("s1a");
    D_flush();
    uint8_t* writePtr = buf;
    D_printf("s1b");
    D_flush();
    if(buf == NULL) {
        D_printf("L2: ERROR: no memory left...");
        return -1;
    }
    D_printf("s2");
    D_flush();

    // SFD
    *writePtr++ = START_FRAME_DELIMITER;

    D_printf("s3");
    D_flush();
    // Sender Name
    *writePtr++ = in.senderNameLength;
    memcpy(writePtr, in.senderName, in.senderNameLength);
    writePtr += in.senderNameLength;
    D_printf("s4");
    D_flush();

    // Payload
    *writePtr++ = in.payloadLength;
    memcpy(writePtr, in.payload, in.payloadLength);
    writePtr += in.payloadLength;

    D_printf("s5");
    D_flush();
    // FCS
    *writePtr++ = 0xA;  // TODO: calculate propper CRC
    D_printf("s6");
    D_flush();

    // EFD
    *writePtr = END_FRAME_DELIMITER;
    D_printf("s7");
    D_flush();

    return totalLength;
}

void layer2TaskFunc(void * pvParameters) {
    Layer2 *thisPtr = static_cast<Layer2*>(pvParameters);       // TODO: This approach does not work and the object becomes invalid after a while, change to basic C

    QueueHandle_t appToLayer2Queue = thisPtr->getAppToLayer2QueueHandle();
    QueueHandle_t layer2ToAppQueue = thisPtr->getLayer2ToAppQueueHandle();
    QueueHandle_t layer2ToRadioQueue= thisPtr->getLayer2ToRadioQueueHandle();
    QueueHandle_t radioToLayer2Queue = thisPtr->getRadioToLayer2QueueHandle();

    Layer2::ProtocolState currentState = Layer2::IDLE;

    ChatMessage userInputMessage;
    icqFrame_t radioReceivedFrame;
    icqFrame_t f;
    uint8_t* serialisedOutputFrame;
    uint8_t serialisedOutputFrameLength;

    for(ever) {
        switch(currentState) {
            case Layer2::IDLE:
                if(xQueueReceive(radioToLayer2Queue, (void*) &radioReceivedFrame, 0 * portTICK_PERIOD_MS) == pdTRUE) {
                    currentState = Layer2::RX;
                }
                if(xQueueReceive(appToLayer2Queue, (void*) &userInputMessage, 0 * portTICK_PERIOD_MS) == pdTRUE) {

                    f.senderNameLength = strlen(userInputMessage.sender);
                    if(f.senderNameLength > MAX_USERNAME_LEN) {
                        D_printf("L2: ERROR: Username too long (%d > %d)! Dropping Message...", f.senderNameLength, MAX_USERNAME_LEN);
                        continue;
                    }

                    f.payloadLength = strlen(userInputMessage.message);
                    if(f.payloadLength > MAX_MSG_LEN) {
                        D_printf("L2: ERROR: Message too long (%d > %d)! Dropping Message...", f.payloadLength, MAX_MSG_LEN);
                        continue;
                    }

                    void* retPtr = NULL;
                    retPtr =  malloc(f.senderNameLength);
                    if(retPtr == NULL) {
                        D_println("E: no mem");
                        continue;
                    }
                    f.senderName = (char*) retPtr;
                    strncpy(f.senderName, userInputMessage.sender, f.senderNameLength);

                    retPtr = malloc(f.payloadLength);
                    if(retPtr == NULL) {
                        D_println("E: no mem");
                        continue;
                    }

                    f.payload = (uint8_t*) retPtr;
                    memcpy(f.payload, userInputMessage.message, MAX_MSG_LEN);

                    D_printf("4");
                    D_flush();

                    serialisedOutputFrameLength = serialise(f, serialisedOutputFrame);
                    D_printf("5");
                    D_flush();
                    if(serialisedOutputFrameLength > 0) {
                        currentState = Layer2::TX;
                    }
                    D_printf("6");
                    D_flush();
                }
                break;
            case Layer2::RX:
                Serial.printf("L2: Receiving Frame...\n");
                // Check for SFD
                // Save all subsequent byts in buffer until EFD received
                // Forward to App
                ChatMessage c;
                strncpy(c.sender, radioReceivedFrame.senderName, MAX_USERNAME_LEN);
                memcpy(c.message, radioReceivedFrame.payload, MAX_MSG_LEN);
                xQueueSend(layer2ToAppQueue, (void*) &c, 0 * portTICK_PERIOD_MS);       // We cannot wait here. If the Queue is full, we have to drop the message and hope for a retransmit
                currentState = Layer2::IDLE;
                break;
            case Layer2::TX:
                // Send Frame to Radio and wait for it to signal completion
                thisPtr->printSerialisedFrame(serialisedOutputFrame, serialisedOutputFrameLength);

                xQueueSend(layer2ToRadioQueue, (void *) serialisedOutputFrame, 0 * portTICK_PERIOD_MS); // We shouldn't have to wait here. The radio must have time for use
                currentState = Layer2::IDLE;
                break;
        }
        vTaskDelay(10 * portTICK_PERIOD_MS);
    }
}

void Layer2::printSerialisedFrame(uint8_t *buf, uint8_t len) {
    for(uint8_t i = 0; i < len; i++) {
        if(i % 8 == 0) {
            D_printf(" ");
        }
        else if(i % 32 == 0) {
            D_printf("\n");
        }
        D_printf("%2X ", buf[i]);
    }
}


Layer2::Layer2(QueueHandle_t upperLayerIn, QueueHandle_t upperLayerOut, QueueHandle_t lowerLayerOut, QueueHandle_t lowerLayerIn) {
    appToLayer2Queue = upperLayerIn;
    layer2ToAppQueue = upperLayerOut;
    layer2ToRadioQueue = lowerLayerOut;
    radioToLayer2Queue = lowerLayerIn;

    xTaskCreatePinnedToCore(layer2TaskFunc, "Layer2 Task", 4096, this, 2, &layer2TaskHandle, 0);
}

int Layer2::sendFrame(icqFrame_t f) {
    Serial.printf("L2: Sending Frame...\n");
    // Loopback for now
    if(xQueueSend(radioToLayer2Queue, &f, 100 * portTICK_PERIOD_MS) == pdTRUE) {
        return 0;
    }
    return -1;
}