#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <vector>
#include "Subroutine.h"

class State
{
public:
    State();
    virtual ~State();
    
    virtual uint32_t config();

    virtual uint32_t onEntry();
    virtual uint32_t onExit();
    virtual uint32_t update();

    virtual uint32_t addOnEntry(Subroutine * p_subroutine);
    virtual uint32_t addOnExit(Subroutine * p_subroutine);
    virtual uint32_t addOnUpdate(Subroutine * p_subroutine);

private:
    std::vector<Subroutine *> m_subroutineOnEntryVector;
    uint16_t m_numSubroutineOnEntry = 0;
    std::vector<Subroutine *> m_subroutineOnExitVector;
    uint16_t m_numSubroutineonExit = 0;
    std::vector<Subroutine *> m_subroutineUpdateVector;
    uint16_t m_numSubroutineUpdate = 0;

    bool m_configuredStateOnEntry;
    bool m_configuredStateOnExit;
    bool m_configuredStateUpdate;
};

#endif // STATE_H
