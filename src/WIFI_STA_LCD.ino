#if 0
#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include <WiFi.h>
// #include "WIFI_Driver.h"

// Set these to your desired credentials.
const char *ssid = "123";
const char *password = "123456789";

int ledblink = 2;

// WiFiServer server(80);
// WiFiClient client;

void setup()
{
  pinMode(ledblink,OUTPUT);
  Config_Init();
  LCD_Init();
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  LCD_SetBacklight(180);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(90);
  LCD_Clear(0xffff);
  delay(2000);     
  LCD_Clear(0x0000);
  delay(1000);  
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_DrawImage(gImage_pic1, 0, 0, 170, 320);
  Serial.print("连接到 WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  // WIFI_AP_Init();
  }
}

void loop(){
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("wifi连接成功");
    digitalWrite(ledblink, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);                      // wait for a second
    digitalWrite(ledblink, LOW);   // turn the LED off by making the voltage LOW
    delay(100);     
  }
  else {
    Serial.println("未连接到WiFi");
    // Serial.plint(......);
    delay(5000);
  }    
  // WiFiClient client = server.available();   // listen for incoming clients
  // WIFI_LCD_Control(client);

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
#endif