#ifndef EC_TASK_FSM_H
#define EC_TASK_FSM_H

#include "Fsm.h"
#include "EcStateClearingFault.h"
#include "EcStateDisabled.h"
#include "EcStateDisabling.h"
#include "EcStateEnabled.h"
#include "EcStateEnabling.h"
#include "EcStateFault.h"
#include "EcStateInitialized.h"
#include "EcStateInitializing.h"
#include "EcStateJoystickControl.h"
#include "EcStateStandby.h"

class EcTaskFsm
{
public:
	EcTaskFsm();

    ~EcTaskFsm();

    uint32_t config();

    uint32_t update();

private:
    Fsm fsm;
};

#endif // EC_TASK_FSM_H
