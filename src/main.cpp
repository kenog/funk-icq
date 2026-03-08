#include <Arduino.h>

#include <ui/UartUI.h>
#include <app/FunkICQ.h>
#include <protocol/Layer2.h>
#include <radio/Dra818.h>
#include <Settings.h>

// Transmission Queues
QueueHandle_t uiToAppQueue;
QueueHandle_t appToLayer2Queue;
QueueHandle_t layer2toToRadioQueue;

// Reception Queues
QueueHandle_t radioToLayer2Queue;
QueueHandle_t layer2ToAppQueue;
QueueHandle_t appToUiQueue;

UartUI* uartUi;
FunkIcq* funkIcq;
Layer2* layer2;

void setup() {

    // Enable USB Serial device for Debugging
    D_SerialBegin(115200);

    uiToAppQueue = xQueueCreate(QUEUE_LENGTH, sizeof(ChatMessage));
    appToUiQueue = xQueueCreate(QUEUE_LENGTH, sizeof(ChatMessage));

    appToLayer2Queue = xQueueCreate(QUEUE_LENGTH, sizeof(ChatMessage));
    layer2ToAppQueue = xQueueCreate(QUEUE_LENGTH, sizeof(ChatMessage));

    layer2toToRadioQueue = xQueueCreate(QUEUE_LENGTH, sizeof(icqFrame_t));
    radioToLayer2Queue = xQueueCreate(QUEUE_LENGTH, sizeof(icqFrame_t));

    // Initialise Application Layer
    funkIcq = new FunkIcq(uiToAppQueue, appToUiQueue, appToLayer2Queue, layer2ToAppQueue);

    delay(2000);

    // Initialise User Interface
    uartUi = new UartUI(SERIAL_2, 115200, uiToAppQueue, appToUiQueue);

    layer2 = new Layer2(appToLayer2Queue, layer2ToAppQueue, layer2toToRadioQueue, radioToLayer2Queue);

    delay(2000);
    // Initialise PHY
    //Dra818 radio(appToRadioQueue, radioToAppQueue);

    D_println("Initialisation Done...");
}

void loop(){
    // Nothing to do here, we do everything in the Tasks we started during setup()
}