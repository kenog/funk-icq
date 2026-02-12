#include <Arduino.h>
#include <IOInterface.h>
#include <string>

enum SerialPortNumber {
    SERIAL_0 = 0,
    SERIAL_1 = 1,
    SERIAL_2 = 2
};

class UartIO : IOInterface {
    public:
        UartIO(SerialPortNumber port, unsigned int baudrate);

        void send(Message);
        Message receive();
        HardwareSerial* getSerialPort();

    private:
        HardwareSerial* serialPort;
        unsigned int baudrate;
        TaskHandle_t readUserInputTask;
};