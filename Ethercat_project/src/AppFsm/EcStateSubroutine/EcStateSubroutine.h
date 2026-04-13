#ifndef EC_STATE_SUBROUTINE_H
#define EC_STATE_SUBROUTINE_H

#include <stdint.h>

class EcStateSubroutine
{
public:
    EcStateSubroutine();
    ~EcStateSubroutine();

    uint32_t config();

    uint32_t invoke();

private:
};

#endif // EC_STATE_SUBROUTINE_H
