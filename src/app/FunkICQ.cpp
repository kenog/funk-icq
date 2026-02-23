#include <app/FunkICQ.h>

FunkIcq::FunkIcq(QueueHandle_t uiToAppQueue) {
  this->uiToAppQueue = uiToAppQueue;

  xTaskCreate(funkIcqTask, "FunkICQ", 4096, this, 1, &appTask);
}

void funkIcqTask(void* pvParams) {
    FunkIcq* thisPtr = static_cast<FunkIcq*>(pvParams);
    Serial.printf("FunkIcq.thisPtr->getQ() = %p\n", thisPtr->getUiToAppQueue());

    ChatMessage transmittableMsg;
    String receivedMessage;
    QueueHandle_t testQ = xQueueCreate(5, sizeof(String));

    Serial.print(receivedMessage);
    while(true) {
        if(xQueueReceive(testQ, (void*) &receivedMessage, 100 * portTICK_PERIOD_MS)) {
          // We have new user input
          Serial.print("new input:");
          Serial.println(receivedMessage);
        }
    }
}