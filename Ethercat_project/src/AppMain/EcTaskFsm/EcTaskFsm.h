#ifndef EC_TASK_FSM_H
#define EC_TASK_FSM_H

#include <vector>

#include "EcStateStandby.h"
#include "EcStateFault.h"
#include "EcStateClearingFault.h"
#include "EcStateInitializing.h"
#include "EcStateInitialized.h"
#include "EcStateReady.h"
#include "EcStateDisabling.h"
#include "EcStateDisabled.h"
#include "EcStateEnabled.h"
#include "EcStateEnabling.h"
#include "EcStateJoystickControl.h"

#include "EcState.h"
#include "Fsm.h"

class EcTaskFsm
{
public:
	EcTaskFsm();

    ~EcTaskFsm();

    uint32_t setTaskAddr(
        EcTaskEthercatSlave* p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl* p_ecTaskRobotControl_,
        EcTaskUser* p_ecTaskUser_,
        EcTaskInterface* p_ecTaskInterface_
    );

    EC_T_DWORD addAllState();

	EC_T_DWORD cleanupState();

    uint32_t config();

    uint32_t update();

private:
    EcStateData::EcTaskAll ecTaskAll;
    std::vector<State*> m_stateVector;
    
    Fsm m_fsm;
};

#endif // EC_TASK_FSM_H
