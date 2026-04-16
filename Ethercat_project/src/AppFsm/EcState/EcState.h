#ifndef EC_STATE_H
#define EC_STATE_H

#include <stdint.h>

#include "EcTaskEthercatSlave.h"
#include "EcTaskEthercatSlaveServo.h"
#include "EcTaskRobotControl.h"
#include "EcTaskUser.h"
#include "EcTaskInterface.h"

namespace EcStateData
{
    enum StateId : uint32_t
    {
        STANDBY = 101,
        FAULT = 102,
        CLEARING_FAULT = 103,
        INITIALIZING = 104,
        INITIALIZED = 105,
        DISABLING = 106,
        DISABLED = 107,
        ENABLING = 108,
        ENABLED = 109,
        READY = 110,
        JOYSTICKCONTROL = 111
    };

    struct EcTaskAll
	{
		EcTaskEthercatSlave* p_ecTaskEthercatSlave;
		EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo;
		EcTaskRobotControl* p_ecTaskRobotControl;
		EcTaskUser* p_ecTaskUser;
		EcTaskInterface* p_ecTaskInterface;
	};
}

#endif // EC_STATE_H
