#include <SPI.h>
#include "ui/lcd/LCD_Driver.h"
#include "ui/lcd/GUI_Paint.h"
#include <radio/Dra818.h>
#include <app/FunkICQ.h>
#include <protocol/FrameCodec.h>

#define QUEUE_LENGTH 5

int ledblink = 2;
uint8_t loopcounter = 0;

QueueHandle_t uiToAppQueue;
QueueHandle_t appToRadioQueue;
QueueHandle_t radioToAppQueue;


void setup()
{

  // Enable USB Serial device for Debugging
  Serial.begin(115200);

  uiToAppQueue = xQueueCreate(QUEUE_LENGTH, sizeof(ChatMessage));
  appToRadioQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Frame));
  radioToAppQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Frame));

  Dra818 radio(appToRadioQueue, radioToAppQueue);
  FunkIcq funkIcq;


  



  pinMode(ledblink,OUTPUT);
  Config_Init();
  LCD_Init();
  Serial.begin(115200);

  LCD_SetBacklight(255);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(270);
  LCD_Clear(0xffff);

  Serial.print("Initialisation Done...");
}

void loop(){

    Serial.printf("loop %d\n", loopcounter++);
    digitalWrite(ledblink, (loopcounter % 2));  // turn the LED on (HIGH is the voltage level)
    char* aks = "Funk ICQ";
    Paint_DrawString_EN(0, 0,aks, &Font24, WHITE, BLUE);
    Paint_DrawString_EN(0, 50, "Keno: ", &Font16, WHITE, RED);
    Paint_DrawString_EN(70, 50, "Hallo Matti und Arne!", &Font16, WHITE, BLACK);
    delay(1000);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
