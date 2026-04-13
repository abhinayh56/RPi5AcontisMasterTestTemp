#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <string>
#include <vector>
#include "Subroutine.h"
#include "Transition.h"

class State
{
public:
    State(const std::string &name, uint32_t id);
    virtual ~State();

    virtual uint32_t config();

    virtual uint32_t onEntry();
    virtual uint32_t onExit();
    virtual uint32_t update();

    virtual uint32_t addOnEntry(Subroutine* p_subroutine);
    virtual uint32_t addOnExit(Subroutine* p_subroutine);
    virtual uint32_t addOnUpdate(Subroutine* p_subroutine);
    virtual uint32_t addTransition(Subroutine* p_subroutine);

    std::string n_name;
    uint32_t m_id;

private:
    std::vector<Subroutine*> m_subroutineOnEntryVector;
    uint16_t m_numSubroutineOnEntry = 0;
    std::vector<Subroutine*> m_subroutineOnExitVector;
    uint16_t m_numSubroutineonExit = 0;
    std::vector<Subroutine*> m_subroutineUpdateVector;
    uint16_t m_numSubroutineUpdate = 0;
    std::vector<Subroutine*> m_transitionVector;
    uint16_t m_numTransition = 0;

    bool m_configuredStateOnEntry;
    bool m_configuredStateOnExit;
    bool m_configuredStateUpdate;
};

#endif // STATE_H
