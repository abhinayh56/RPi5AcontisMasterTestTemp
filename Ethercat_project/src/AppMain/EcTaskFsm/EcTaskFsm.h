#ifndef EC_TASK_FSM_H
#define EC_TASK_FSM_H

#include "Fsm.h"

#include"EcTaskEthercatSlave.h"
#include"EcTaskEthercatSlaveServo.h"
#include"EcTaskRobotControl.h"
#include"EcTaskUser.h"
#include"EcTaskInterface.h"

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


class EcTaskFsm
{
public:
	EcTaskFsm();

    ~EcTaskFsm();

    uint32_t config(
        EcTaskEthercatSlave* p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl* p_ecTaskRobotControl_,
        EcTaskUser* p_ecTaskUser_,
        EcTaskInterface* p_ecTaskInterface_
    );

    uint32_t update();

private:
    // Fsm fsm;

    // fsm test
    Data_store_element<bool> m_InputCh_1;
	Data_store_element<bool> m_InputCh_2;
	Data_store_element<bool> m_InputCh_3;
	Data_store_element<bool> m_InputCh_4;
	Data_store_element<bool> m_InputCh_5;
	Data_store_element<bool> m_InputCh_6;
	Data_store_element<bool> m_InputCh_7;
	Data_store_element<bool> m_InputCh_8;

    bool m_data_1 = false;
    bool m_data_2 = false;
    bool m_data_3 = false;
    bool m_data_4 = false;
    bool m_data_5 = false;
    bool m_data_6 = false;
    bool m_data_7 = false;
    bool m_data_8 = false;

    void init_fsm();
    void update_fsm();

    EcTaskEthercatSlave* p_ecTaskEthercatSlave;
    EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo;
    EcTaskRobotControl* p_ecTaskRobotControl;
    EcTaskUser* p_ecTaskUser;
    EcTaskInterface* p_ecTaskInterface;
};

#endif // EC_TASK_FSM_H
