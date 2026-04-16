#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <string>
#include <vector>
#include "../Subroutine/Subroutine.h"
#include "EcState.h"

class State
{
public:
    State(const std::string &name, uint32_t id);
    virtual ~State();
    virtual uint32_t addOnEntry(Subroutine* p_subroutine);
    virtual uint32_t addTransition(SubroutineTransition* p_subroutine);
    virtual uint32_t addCallback(Subroutine* p_subroutine);
    virtual uint32_t addOnExit(Subroutine* p_subroutine);
    virtual uint32_t setTaskAddr(
        EcTaskEthercatSlave *p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl *p_ecTaskRobotControl_,
        EcTaskUser *p_ecTaskUser_,
        EcTaskInterface *p_ecTaskInterface_);
    virtual uint32_t config();
    virtual uint32_t update(uint32_t &nextStateId);
    
    std::string m_name;
    uint32_t m_id;

private:
    std::vector<Subroutine*> m_onEntryVector;
    std::vector<SubroutineTransition*> m_transitionVector;
    std::vector<Subroutine*> m_callbackVector;
    std::vector<Subroutine*> m_onExitVector;

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

    EcStateData::EcTaskAll ecTaskAll;
};

#endif // STATE_H
