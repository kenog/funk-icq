#pragma once

#define ever ;;
#define MAX_FRAME_LEN 255
#define MAX_USERNAME_LEN 10
#define MAX_MSG_LEN (MAX_FRAME_LEN - MAX_USERNAME_LEN)
#define QUEUE_LENGTH 5

#define DEBUG 1    // SET TO 0 OUT TO REMOVE TRACES

#if DEBUG
#define D_SerialBegin(...)  Serial.begin(__VA_ARGS__);
#define D_flush(...)        Serial.flush(__VA_ARGS__)
#define D_print(...)        Serial.print(__VA_ARGS__)
#define D_printf(...)       Serial.printf(__VA_ARGS__)
#define D_println(...)      Serial.println(__VA_ARGS__)
#define D_write(...)        Serial.write(__VA_ARGS__)
#else
#define D_SerialBegin(...)
#define D_flush(...)
#define D_print(...)
#define D_printf(...)
#define D_println(...)
#define D_write(...)
#endif

typedef struct {
    char username[MAX_USERNAME_LEN];
} settings_t;

static settings_t globalSettings;
