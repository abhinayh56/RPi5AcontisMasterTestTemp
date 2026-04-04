#ifndef EC_TASK_MANAGER_H
#define EC_TASK_MANAGER_H

#include "EcTaskEthercatSlave.h"
#include "EcTaskRobotControl.h"
#include "EcTaskUser.h"

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
};

#endif // EC_TASK_MANAGER_H
