#ifndef EC_TASK_ROBOT_CONTROL_BASE_H
#define EC_TASK_ROBOT_CONTROL_BASE_H

#include "EcType.h"
#include "ITC.h"

class EcTaskRobotControlBase
{
public:
    EcTaskRobotControlBase(uint16_t ecTaskRobotControlId, const std::string &ecTaskRobotControlName);

    virtual ~EcTaskRobotControlBase();

    virtual EC_T_DWORD registerPublisher() = 0;

    virtual EC_T_DWORD registerSubscriber() = 0;

    virtual EC_T_DWORD mainProcess() = 0;

    uint16_t getTaskId();

    std::string &getTaskName();

protected:
    uint16_t m_ecTaskRobotControlId;
    std::string m_ecTaskRobotControlName;
};

#endif // EC_TASK_ROBOT_CONTROL_BASE_H
