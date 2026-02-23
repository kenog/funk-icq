#include <Arduino.h>

#include <ui/UartUI.h>
#include <app/FunkICQ.h>
#include <protocol/Layer2.h>
#include <radio/Dra818.h>

#define QUEUE_LENGTH 5

int ledblink = 2;
uint8_t loopcounter = 0;

// Transmission Queues
QueueHandle_t uiToAppQueue;
QueueHandle_t appToLayer2Queue;
QueueHandle_t layer2toToRadioQueue;

// Reception Queues
QueueHandle_t radioToLayer2Queue;
QueueHandle_t layer2ToAppQueue;
QueueHandle_t appToUiQueue;

void setup()
{

  // Enable USB Serial device for Debugging
  Serial.begin(115200);

  uiToAppQueue = xQueueCreate(QUEUE_LENGTH, sizeof(String));
  Serial.printf("main.Q = %p\n", uiToAppQueue);
  //appToRadioQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Frame));
  //radioToAppQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Frame));

  // Initialise Application Layer
  FunkIcq funkIcq(uiToAppQueue);

  // Initialise User Interface
  //UartUI uartUi(SERIAL_0, 115200, uiToAppQueue);

  // Initialise PHY
  //Dra818 radio(appToRadioQueue, radioToAppQueue);

  Serial.println("Initialisation Done...");
}

void loop(){
  Serial.println("Loop...");
  delay(1000);
}