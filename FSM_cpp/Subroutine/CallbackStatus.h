#ifndef CALLBACK_STATUS_H
#define CALLBACK_STATUS_H

#include <stdint.h>

enum CallbackStatus : uint32_t
{
    SUCCESS = 0,
    IN_PROGRESS = 1,
    FAULT = 2
};

#endif // CALLBACK_STATUS_H
