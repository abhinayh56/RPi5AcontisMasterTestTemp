#ifndef EC_STATE_H
#define EC_STATE_H

#include <stdint.h>

class EcState
{
public:
    EcState(){}

    ~EcState(){}

    uint32_t onEntry(){}

    uint32_t invoke(){}
    
    uint32_t onExit(){}

private:
};

#endif // EC_STATE_H