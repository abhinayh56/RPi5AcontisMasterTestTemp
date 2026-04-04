#ifndef EC_TASK_USER_BASE_H
#define EC_TASK_USER_BASE_H

#include "EcType.h"
#include "ITC.h"

class EcTaskUserBase
{
public:
    EcTaskUserBase(uint16_t slaveAddr, const std::string &slaveName);

    virtual ~EcTaskUserBase();

    virtual EC_T_DWORD registerPublisher() = 0;

    virtual EC_T_DWORD registerSubscriber() = 0;

    virtual EC_T_DWORD mainProcess() = 0;
};

#endif // EC_TASK_USER_BASE_H
