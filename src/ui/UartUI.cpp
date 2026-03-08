#include <Arduino.h>
#include <ui/UartUI.h>

#define ever ;;

void readUserInput(void * pvParameters) {
    UartUI *thisPtr = static_cast<UartUI*>(pvParameters);       // TODO: This approach does not work and the object becomes invalid after a while, change to basic C

    QueueHandle_t uiToAppQueueHandle = thisPtr->getUiToAppQueueHandle();
    D_printf("UartUI: uiToAppQueueHandle = %p\n", uiToAppQueueHandle);

    HardwareSerial* serialPtr = thisPtr->getSerialPort();
    D_printf("UartUI: serialPtr = %p\n", serialPtr);

    ChatMessage userInput;
    char* userInputWritePtr = &userInput.message[0];
    bool startNewMessage = true;

    ChatMessage receivedMsg;

    for(ever) {
        if(startNewMessage) {
            //serialPtr->print("\n\nEnter Message: ");      // Commented because it messes up UI when messages are received
            startNewMessage = false;
        }

        // Check for new user input
        while (serialPtr->available() && userInputWritePtr < userInput.message + sizeof(char) * (MAX_INPUT - 1)) {
            char incomingChar = serialPtr->read();  // Read each character from the buffer

            if (incomingChar == '\n' || incomingChar == '\r') {
                serialPtr->println();    // Go to next line, so potentially received messages are not printed behind the user input
                *userInputWritePtr++ = '\0';                                // User Message is over - add 0-termination
                D_printf("UartUI: New user input: '%s'\n", userInput.message);

                xQueueSend(uiToAppQueueHandle, (void*) &userInput, 1000);    // xQueueReceive copys userInput into its own buffer so we can directly reuse userInput

                userInputWritePtr = &userInput.message[0];                          // Reset write pointer to beginning of buffer
                startNewMessage = true;
            }
            else {
                // Normal character
                *userInputWritePtr++ = incomingChar;    // Store in input buffer
                serialPtr->print(incomingChar);         // Echo back to termial for user convenience
            }
        }

        // Check if we received anything we should display
        if(xQueueReceive(thisPtr->getAppToUiQueueHandle(), (void*) &receivedMsg, 0 * portTICK_PERIOD_MS) == pdTRUE) {
            thisPtr->printMessage(receivedMsg);
        }

        // What we do here is not urgent usually. Pause a bit to give other Tasks some CPU time.
        vTaskDelay(100 * portTICK_PERIOD_MS);
    }
}

UartUI::UartUI(SerialPortNumber port, unsigned int _baudrate, QueueHandle_t _uiToAppQueue, QueueHandle_t _appToUiQueue) {
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
            D_printf("ERROR: Unknown Serial Port: %d\n", port);
            D_flush();
    }

    D_printf("UI: Set Serial Port: %d, addr = %p\n", port, getSerialPort());

    baudrate = _baudrate;
    serialPort->begin(baudrate);

    uiToAppQueue = _uiToAppQueue;
    appToUiQueue = _appToUiQueue;

    xTaskCreatePinnedToCore(readUserInput, "UART User Input", 4096, this, 2, &readUserInputTask, 0);
}

void UartUI::printMessage(const ChatMessage& msg) {
    serialPort->printf("%s: %s\n", msg.sender, msg.message);
}

HardwareSerial* UartUI::getSerialPort() {
    return serialPort;
}