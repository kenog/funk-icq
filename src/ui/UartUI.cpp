#include <Arduino.h>
#include <ui/UartUI.h>

#define ever ;;

void readUserInput(void * pvParameters) {
    UartUI *thisPtr = static_cast<UartUI*>(pvParameters);
    Serial.printf("UartUi: uiToAppQueue = %p\n", thisPtr->getUiToAppQueueHandle());

    QueueHandle_t appQ = thisPtr->getUiToAppQueueHandle();
    Serial.printf("UartUi: appQ = %p\n", appQ);


    String userInput = "";
    if(userInput.reserve(MAX_MSG_LEN) == pdFALSE) {
        Serial.println("UI: no mem");
    }
    unsigned int ret;

    HardwareSerial* serialPtr = thisPtr->getSerialPort();
    Serial.printf("UartUi: serialPtr = %p\n", serialPtr);
    bool printed = false;

    for(ever) {
        if(!serialPtr) {
            Serial.println("serialPtr became NULL. Waiting 1s...");
            vTaskDelay(1000 * portTICK_PERIOD_MS);
            serialPtr = thisPtr->getSerialPort();
            continue;
        }
        if(!printed) {
            serialPtr->print("Enter Message: ");
            printed = true;
        }

        while (serialPtr->available()) {
            char incomingChar = serialPtr->read();  // Read each character from the buffer
            userInput += incomingChar;
            serialPtr->print(incomingChar);

            if (incomingChar == '\n' || incomingChar == '\r') {  // Check if the user pressed Enter (new line character)
                // Forward User Input to Application
                serialPtr->print(incomingChar);
                String* outputMessagePtr = new String(userInput);
                Serial.println("UI: user input = '" + userInput + "'");
                Serial.flush();
                xQueueSend(appQ, (void*) outputMessagePtr, 1000);
                //xQueueSend(thisPtr->getUiToAppQueueHandle(), (void*) outputMessagePtr, 1000);

                // Clear the message buffer for the next input
                userInput = "";
                printed = false;
            }
        }
        vTaskDelay(50 * portTICK_PERIOD_MS);
    }
}

UartUI::UartUI(SerialPortNumber port, unsigned int _baudrate, QueueHandle_t _uiToAppQueue) {
    switch(port) {
        case SERIAL_0:
            serialPort = &Serial;
            break;
        case SERIAL_1:
            serialPort = &Serial1;
            break;
        case SERIAL_2:
            serialPort = &Serial2;
            break;
        default:
            Serial.printf("ERROR: Unknown Serial Port: %d\n", port);
    }

    Serial.printf("UI: Set Serial Port: %d, addr = %p\n", port, getSerialPort());

    baudrate = _baudrate;
    uiToAppQueue = _uiToAppQueue;

    Serial.printf("UartUi.this = %p\n", this);

    // Wait 1s until everything is available
    vTaskDelay(1000 * portTICK_PERIOD_MS);

    serialPort->begin(baudrate);

    xTaskCreatePinnedToCore(readUserInput, "UART User Input", 4096, this, 2, &readUserInputTask, 0);
}

void UartUI::printMessage(ChatMessage msg) {
    serialPort->print(msg.sender + ": " + msg.message);
}

HardwareSerial* UartUI::getSerialPort() {
    return serialPort;
}