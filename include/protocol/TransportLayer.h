#pragma once

#include <Arduino.h>
#include <ChatMessage.h>
#include <protocol/FrameCodec.h>

class TransportLayer {
    public:
        void sendMessage(const ChatMessage& msg);
        void onFrameReceived(const Frame& frame);
};