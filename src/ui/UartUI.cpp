#include <Arduino.h>
#include <ui/UartUI.h>

#define ever ;;

void readUserInput(void * pvParameters) {
    UartUI *thisPtr = static_cast<UartUI*>(pvParameters);
    Serial.printf("UartUi.thisPtr = %p", thisPtr);

    String userInput;

    for(ever) {
        if(thisPtr->getSerialPort()->available()) {
            userInput = thisPtr->getSerialPort()->readStringUntil('\n');

            // Forward User Input to Application
            xQueueSend(thisPtr->getUiToAppQueueHandle(), (void*) &userInput, 1000);
        }
    }
}

UartUI::UartUI(SerialPortNumber port, unsigned int baudrate, QueueHandle_t _uiToAppQueue) {
    switch(port) {
        case SERIAL_0:
            this->serialPort = &Serial;
        case SERIAL_1:
            this->serialPort = &Serial1;
        case SERIAL_2:
            this->serialPort = &Serial2;
    }

    serialPort->begin(baudrate);
    this->baudrate = baudrate;
    this->uiToAppQueue = _uiToAppQueue;

    xTaskCreate(readUserInput, "UART User Input", 1000, this, 2, &readUserInputTask);
}

void UartUI::printMessage(ChatMessage msg) {
    serialPort->print(msg.sender + ": " + msg.message);
}

HardwareSerial* UartUI::getSerialPort() {
    return serialPort;
}