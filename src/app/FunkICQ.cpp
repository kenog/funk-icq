#include <app/FunkICQ.h>

FunkIcq::FunkIcq(QueueHandle_t uiToAppQueue) {
  this->uiToAppQueue = uiToAppQueue;

  // Wait 1s until everything is available
  vTaskDelay(1000 * portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(funkIcqTask, "FunkICQ", 4096, this, 1, &appTask, 0);
  Serial.println("APP: Task created");
  Serial.flush();
}

void funkIcqTask(void* pvParams) {
    Serial.println("APP: Task started");
    FunkIcq* thisPtr = static_cast<FunkIcq*>(pvParams);
    if(!thisPtr) {
      Serial.println("APP: thisPtr is NULL");
    }
    else {
        Serial.printf("FunkIcq.thisPtr->getQ() = %p\n", thisPtr->getUiToAppQueue());
    }
    Serial.flush();


    ChatMessage transmittableMsg;
    String* receivedMessagePtr = NULL;
    QueueHandle_t uiToAppQueue = thisPtr->getUiToAppQueue();

    while(true) {
        if(!thisPtr) {
          Serial.println("APP: thisPtr is NULL");
          continue;
        }
        else {
          Serial.println("APP: thisPtr OK");
        }
        if(!uiToAppQueue) {
          Serial.println("APP: queue is NULL");
          continue;
        }
        else {
          Serial.println("APP: queue OK");
        }

        /*
        if(xQueueReceive(uiToAppQueue, (void*) &receivedMessagePtr, 5000 * portTICK_PERIOD_MS)) {
          // We have new user input
          Serial.print("new input:");
          Serial.println(*receivedMessagePtr);
          Serial.flush();
          free(receivedMessagePtr);
        }
        else {
          Serial.println("APP: no input");
        }
*/
        vTaskDelay(3000 * portTICK_PERIOD_MS);
    }
}