#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <string>
#include <vector>
#include "../SubroutineBase/SubroutineBase.h"

class StateBase
{
public:
    StateBase(uint32_t id, const std::string &name);
    virtual ~StateBase();
    virtual uint32_t addOnEntry(SubroutineBase* p_subroutine);
    virtual uint32_t addTransition(SubroutineTransitionBase* p_subroutine);
    virtual uint32_t addCallback(SubroutineBase* p_subroutine);
    virtual uint32_t addOnExit(SubroutineBase* p_subroutine);
    virtual uint32_t config();
    virtual uint32_t update(uint32_t &nextStateId);
    
protected:
    uint32_t m_id;
    std::string m_name;
    TaskAll* p_taskAll;

private:
    std::vector<SubroutineBase*> m_onEntryVector;
    std::vector<SubroutineTransitionBase*> m_transitionVector;
    std::vector<SubroutineBase*> m_callbackVector;
    std::vector<SubroutineBase*> m_onExitVector;

    uint32_t m_numOnEntry;
    uint32_t m_numTransition;
    uint32_t m_numCallback;
    uint32_t m_numOnExit;

    uint32_t m_idNext = 0;
    bool m_onEntryFlag = false;

    uint32_t onEntry();
    uint32_t transition(uint32_t &nextStateId);
    uint32_t callback();
    uint32_t onExit();
};

#endif // STATE_H
