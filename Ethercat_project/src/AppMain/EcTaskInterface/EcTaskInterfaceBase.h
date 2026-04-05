#ifndef EC_TASK_INTERFACE_BASE_H
#define EC_TASK_INTERFACE_BASE_H

#include "EcType.h"
#include "EcError.h"
#include "ITC.h"

class EcTaskInterfaceBase
{
public:
    EcTaskInterfaceBase(uint16_t ecTaskInterfaceId, const std::string &ecTaskInterfaceName);

    virtual ~EcTaskInterfaceBase();

    virtual EC_T_DWORD registerPublisher() = 0;

    virtual EC_T_DWORD registerSubscriber() = 0;

    virtual EC_T_DWORD publishData() = 0;

    virtual EC_T_DWORD subscribeData() = 0;

    virtual EC_T_DWORD mainProcess() = 0;

    uint16_t getTaskId();

    std::string &getTaskName();

protected:
    uint16_t m_ecTaskInterfaceId;
    std::string m_ecTaskInterfaceName;
};

#endif // EC_TASK_INTERFACE_BASE_H
