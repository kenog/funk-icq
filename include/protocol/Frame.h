#pragma once

#include <Arduino.h>
#include <Settings.h>

constexpr uint8_t START_FRAME_DELIMITER = 0x66;
constexpr uint8_t END_FRAME_DELIMITER = 0x66;

typedef struct {
    uint8_t startFrameDelimiter;
    uint8_t senderNameLength;
    char* senderName;
    uint8_t payloadLength;
    uint8_t* payload;
    uint8_t fcs;
    uint8_t endFrameDelimiter;
} icqFrame_t;