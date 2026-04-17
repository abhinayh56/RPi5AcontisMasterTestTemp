#ifndef EC_TASK_USER_BASE_H
#define EC_TASK_USER_BASE_H

#include "EcType.h"
#include "EcError.h"
#include "ITC.h"

class EcTaskUserBase
{
public:
    EcTaskUserBase(uint16_t ecTaskUserId, const std::string &ecTaskUserName);

    virtual ~EcTaskUserBase();

    virtual EC_T_DWORD registerPublisher() = 0;

    virtual EC_T_DWORD registerSubscriber() = 0;

    virtual EC_T_DWORD publishData() = 0;

    virtual EC_T_DWORD subscribeData() = 0;

    virtual EC_T_DWORD mainProcess() = 0;

    uint16_t getTaskId();

    std::string &getTaskName();

protected:
    uint16_t m_ecTaskUserId;
    std::string m_ecTaskUserName;
};

#endif // EC_TASK_USER_BASE_H
