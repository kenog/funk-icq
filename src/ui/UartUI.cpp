#include <Arduino.h>
#include <ui/UartUI.h>

#define ever ;;

void readUserInput(void * pvParameters) {
    UartUI *thisPtr = static_cast<UartUI*>(pvParameters);
    Serial.printf("UartUi: uiToAppQueue = %p\n", thisPtr->getUiToAppQueueHandle());

    QueueHandle_t appQ = thisPtr->getUiToAppQueueHandle();
    Serial.printf("UartUi: appQ = %p\n", appQ);


    char userInput[MAX_INPUT];

    char* userInputWritePtr = &userInput[0];
    unsigned int ret;

    HardwareSerial* serialPtr = thisPtr->getSerialPort();
    Serial.printf("UartUi: serialPtr = %p\n", serialPtr);
    bool startNewMessage = true;

    for(ever) {
        if(!serialPtr) {
            Serial.println("serialPtr became NULL. Waiting 1s...");
            vTaskDelay(1000 * portTICK_PERIOD_MS);
            serialPtr = thisPtr->getSerialPort();
            continue;
        }
        if(startNewMessage) {
            serialPtr->print("Enter Message: ");
            startNewMessage = false;
        }

        while (serialPtr->available() && userInputWritePtr < userInput + sizeof(char) * (MAX_INPUT - 1)) {
            char incomingChar = serialPtr->read();  // Read each character from the buffer

            if (incomingChar == '\n' || incomingChar == '\r') {  // Check if the user pressed Enter (new line character)
                // Forward User Input to Application
                *userInputWritePtr++ = '\0';    // User Message is over - add 0-termination
                Serial.printf("UI: New user input: '%s'\n", userInput);
                Serial.flush();
                xQueueSend(appQ, (void*) userInput, 1000);          // xQueueReceive copys userInput into its own buffer so we can directly reuse userInput
                //xQueueSend(thisPtr->getUiToAppQueueHandle(), (void*) userInput, 1000);

                userInputWritePtr = &userInput[0];    // Reset write pointer to beginning of buffer
                startNewMessage = true;
            }
            else {
                // Normal character
                *userInputWritePtr++ = incomingChar;    // Store in input buffer
                serialPtr->print(incomingChar);         // Echo back to termial for user convenience
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