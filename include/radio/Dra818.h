#include <Arduino.h>
#include <protocol/FrameCodec.h>

void dra818TxTask(void* pvParams);

class Dra818 {
    public:

        enum PowerMode {
            HIGH_POWER,
            LOW_POWER
        };

        struct taskParams {
            QueueHandle_t queuehandle;
        };

        Dra818() = delete;
        Dra818(QueueHandle_t in, QueueHandle_t out);

        bool sendBytes(uint8_t* buffer, size_t maxLen);
        unsigned int readBytes(uint8_t* buffer, size_t maxLen);

        bool setPowerMode(PowerMode mode);
        
        const QueueHandle_t& getFromUpperLayerQueueHandle() {
            return fromUpperLayerQueue;
        };

        const QueueHandle_t& getToUpperLayerQueueHandle() {
            return toUpperLayerQueue;
        };

    private:
        void enterTxMode(); // Enable PTT Button
        PowerMode powerMode;
        QueueHandle_t fromUpperLayerQueue;
        QueueHandle_t toUpperLayerQueue;
};