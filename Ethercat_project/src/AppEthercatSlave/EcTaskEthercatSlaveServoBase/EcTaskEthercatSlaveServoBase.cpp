#include "EcTaskEthercatSlaveServoBase.h"

EcTaskEthercatSlaveServoBase::EcTaskEthercatSlaveServoBase(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcTaskEthercatSlaveServoBase::~EcTaskEthercatSlaveServoBase()
{
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::registerTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::registerRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::transferTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::transferRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::processTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::processRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServoBase::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

void EcTaskEthercatSlaveServoBase::dispTxPdo()
{
}

void EcTaskEthercatSlaveServoBase::dispRxPdo()
{
}