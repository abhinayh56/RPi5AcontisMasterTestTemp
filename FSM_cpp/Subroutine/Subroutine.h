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
    virtual uint32_t update();

    bool m_configuredSubroutine;
};

#endif // SUBROUTINE_H
