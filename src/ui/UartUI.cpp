#include <Arduino.h>
#include <ui/UartUI.h>

#define ever ;;

UartUI::UartUI(SerialPortNumber port, unsigned int baudrate) {
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
    xTaskCreate(readUserInput, "UART User Input", 1000, this, 2, &readUserInputTask);
}

void UartUI::send(ChatMessage msg) {

    //serialPort->println(msg.data);
}

ChatMessage UartUI::receive() {
}

void readUserInput(void * pvParameters) {
    UartUI *thisPtr = static_cast<UartUI*>(pvParameters);

    String userInput;

    for(ever) {
        if(thisPtr->getSerialPort()->available()) {
            userInput = thisPtr->getSerialPort()->readStringUntil('\n');
            // TODO: Do something now
        }
        delay(100); // Wait 100ms for new user input
    }
}

HardwareSerial* UartUI::getSerialPort() {
    return serialPort;
}