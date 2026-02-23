#pragma once

#include <Arduino.h>
#include <protocol/Frame.h>

#define MAX_FRAME_LEN 255

class Layer2 {
    public:
        //std::vector<uint8_t> encode(const Frame& frame);
        bool decode(uint8_t bla);
};