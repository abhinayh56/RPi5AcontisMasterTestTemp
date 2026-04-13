#ifndef TRANSITION_H
#define TRANSITION_H

#include <stdint.h>

class Transition
{
public:
    Transition();
    virtual ~Transition();
    virtual uint32_t condition();
    virtual uint32_t target();
};

#endif // TRANSITION_H
