#pragma once

#include <Arduino.h>
#include <ui/IUserInterface.h>
#include <ChatMessage.h>

enum SerialPortNumber {
    SERIAL_0 = 0,
    SERIAL_1 = 1,
    SERIAL_2 = 2
};

class UartUI : IUserInterface {
    public:
        UartUI(SerialPortNumber port, unsigned int baudrate);

        void send(ChatMessage msg);
        ChatMessage receive();

        HardwareSerial* getSerialPort();

    private:
        HardwareSerial* serialPort;
        unsigned int baudrate;
        TaskHandle_t readUserInputTask;
};