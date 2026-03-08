#pragma once

#include <Arduino.h>
#include <ChatMessage.h>


class IUserInterface {
    public:
    virtual void printMessage(const ChatMessage& msg) = 0;
    //virtual void onUserInput(std::function<void(String)> callback) = 0;
};