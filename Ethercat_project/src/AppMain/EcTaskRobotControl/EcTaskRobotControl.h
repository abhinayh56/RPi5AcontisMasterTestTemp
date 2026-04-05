#ifndef EC_TASK_ROBOT_CONTROL_H
#define EC_TASK_ROBOT_CONTROL_H

#include <vector>
#include "EcTaskRobotControlBase.h"

class EcTaskRobotControl
{
public:
    EcTaskRobotControl();

    ~EcTaskRobotControl();

    EC_T_DWORD addAllTaskRobotControl();

    EC_T_DWORD cleanupTask();
    
	EC_T_DWORD registerPublisher();

	EC_T_DWORD registerSubscriber();

    EC_T_DWORD publishData();

    EC_T_DWORD subscribeData();

    EC_T_DWORD mainProcess();

private:
    std::vector<EcTaskRobotControlBase*> m_ecTaskRobotControlBaseVector;
    int m_numTaskRobotControl = 0;

    EC_T_DWORD addTaskRobotControl(EcTaskRobotControlBase* pTaskRobotControl);
};

#endif // EC_TASK_ROBOT_CONTROL_H
