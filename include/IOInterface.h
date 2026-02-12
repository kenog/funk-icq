#include <inttypes.h>
#include <string>

class Message {
    public:
    std::string sender;
    uint8_t *data;
    unsigned int receptionTime;
};

class IOInterface {
    public:
    virtual void send(Message msg);
    virtual Message receive();
};