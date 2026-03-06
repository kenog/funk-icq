#pragma once

#define ever ;;
#define MAX_FRAME_LEN 255
#define MAX_USERNAME_LEN 10
#define MAX_MSG_LEN (MAX_FRAME_LEN - MAX_USERNAME_LEN)
#define QUEUE_LENGTH 5

typedef struct {
    char username[MAX_USERNAME_LEN];
} settings_t;

static settings_t globalSettings;
