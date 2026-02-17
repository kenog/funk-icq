#pragma once

#include <Arduino.h>

#define MAX_FRAME_LEN 255

struct Frame {
    uint8_t sender;
    uint8_t receiver;
    uint8_t length;
    uint8_t payload[MAX_FRAME_LEN];
};

class FrameCodec {
    public:
        //std::vector<uint8_t> encode(const Frame& frame);
        bool decode(uint8_t bla);
};