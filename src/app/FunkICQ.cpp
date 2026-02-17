#include <app/FunkICQ.h>

FunkIcq::FunkIcq() {
  xTaskCreate(funkIcqTask, "FunkICQ", 4096, NULL, 1, NULL);
}

void funkIcqTask(void* pvParams) {
    ChatMessage transmittableMsg;
    ChatMessage receivedMessage;

    while(true) {
        ;//if(xQueueReceive(uiToApp))
    }
}