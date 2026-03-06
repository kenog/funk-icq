#pragma once

#include <Arduino.h>
#include <Settings.h>

struct Frame {
    char sender[MAX_USERNAME_LEN];
    uint8_t receiver;
    uint8_t length;
    uint8_t payload[MAX_FRAME_LEN];
};