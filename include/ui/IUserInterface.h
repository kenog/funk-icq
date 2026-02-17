#pragma once

#include <Arduino.h>
#include <ChatMessage.h>


class IUserInterface {
    public:
    virtual void printMessage(ChatMessage msg);
    virtual void onUserInput(std::function<void(ChatMessage)> callback);
};