#pragma once

#include <Arduino.h>
#include <protocol/Frame.h>

#define MAX_FRAME_LEN 255

class Layer2 {
    public:
        Layer2(QueueHandle_t upperLayerIn, QueueHandle_t upperLayerOut, QueueHandle_t lowerLayerIn, QueueHandle_t lowerLayerOut);

        int sendFrame(icqFrame_t f);
        //int serialise(const icqFrame_t& f, uint8_t* framebuf);
        void printSerialisedFrame(uint8_t *buf, uint8_t len);

        QueueHandle_t getAppToLayer2QueueHandle() { return appToLayer2Queue; };
        QueueHandle_t getLayer2ToAppQueueHandle() { return layer2ToAppQueue; };
        QueueHandle_t getRadioToLayer2QueueHandle() { return radioToLayer2Queue; };
        QueueHandle_t getLayer2ToRadioQueueHandle() { return layer2ToRadioQueue; };

        enum ProtocolState {
            IDLE,
            RX,
            TX
        };

    private:
        TaskHandle_t layer2TaskHandle;

        QueueHandle_t appToLayer2Queue;
        QueueHandle_t layer2ToAppQueue;

        QueueHandle_t layer2ToRadioQueue;
        QueueHandle_t radioToLayer2Queue;
};