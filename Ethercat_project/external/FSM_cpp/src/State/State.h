#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <string>
#include <vector>
#include "../Subroutine/Subroutine.h"

class State
{
public:
    State(const std::string &name, uint32_t id);
    virtual ~State();
    virtual uint32_t addOnEntry(Subroutine* p_subroutine);
    virtual uint32_t addTransition(SubroutineTransition* p_subroutine);
    virtual uint32_t addCallback(Subroutine* p_subroutine);
    virtual uint32_t addOnExit(Subroutine* p_subroutine);
    virtual uint32_t config();
    virtual uint32_t update(uint32_t &nextStateId);
    
    std::string m_name;
    uint32_t m_id;

private:
    std::vector<Subroutine*> m_onEntryVector;
    std::vector<SubroutineTransition*> m_transitionVector;
    std::vector<Subroutine*> m_callbackVector;
    std::vector<Subroutine*> m_onExitVector;

    uint32_t m_numOnEntry = 0;
    uint32_t m_numTransition = 0;
    uint32_t m_numCallback = 0;
    uint32_t m_numOnExit = 0;

    uint32_t m_idNext;
    bool m_onEntryFlag;

    uint32_t onEntry();
    uint32_t transition(uint32_t &nextStateId);
    uint32_t callback();
    uint32_t onExit();
};

#endif // STATE_H
