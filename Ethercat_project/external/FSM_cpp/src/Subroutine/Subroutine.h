#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include <stdint.h>
#include "CallbackStatus.h"

class Subroutine
{
public:
    Subroutine();
    virtual ~Subroutine();
    virtual uint32_t config();
    virtual uint32_t callback();
};

class SubroutineTransition
{
public:
    SubroutineTransition();
    virtual ~SubroutineTransition();
    virtual uint32_t config();
    virtual uint32_t callback(uint32_t &nextStateId);
};

#endif // SUBROUTINE_H
