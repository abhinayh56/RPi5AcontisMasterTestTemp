#ifndef EC_TASK_MANAGER_H
#define EC_TASK_MANAGER_H

#include "EcTaskEthercatSlave.h"
#include "EcTaskEthercatSlaveServo.h"
#include "EcTaskRobotControl.h"
#include "EcTaskUser.h"
#include "EcTaskInterface.h"

class EcTaskManager
{
public:
    EcTaskManager();

    ~EcTaskManager();

    EC_T_DWORD initTask();

    EC_T_DWORD preapareTask();

    EC_T_DWORD setupTask();

    EC_T_DWORD cyclicTask();

    EC_T_DWORD diagnosisTask();

    EC_T_DWORD notifyTask();

private:
    EcTaskEthercatSlave ecTaskEthercatSlave;
    EcTaskEthercatSlaveServo ecTaskEthercatSlaveServo;
    EcTaskRobotControl ecTaskRobotControl;
    EcTaskUser ecTaskUser;
    EcTaskInterface ecTaskInterface;

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
};

#endif // EC_TASK_MANAGER_H
