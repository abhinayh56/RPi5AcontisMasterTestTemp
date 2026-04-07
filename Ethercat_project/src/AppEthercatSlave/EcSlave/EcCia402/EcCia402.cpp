#include "EcCia402.h"

EcCia402::EcCia402(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcCia402::~EcCia402()
{
}

EC_T_DWORD EcCia402::registerTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::registerRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::transferTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::transferRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::processTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::processRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

void EcCia402::dispTxPdo()
{
}

void EcCia402::dispRxPdo()
{
}

EC_T_DWORD EcCia402::checkFault()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::clearFault()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::enable()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::disable()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::setOffsetPosition(int32_t offsetPosition)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_offset.position = offsetPosition;

    return dwRes;
}

EC_T_DWORD EcCia402::setOffsetVelocity(int32_t offsetVelocity)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_offset.velocity = offsetVelocity;

    return dwRes;
}

EC_T_DWORD EcCia402::setOffsetTorque(int16_t offsetTorque)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_offset.torque = offsetTorque;

    return dwRes;
}

EC_T_DWORD EcCia402::setModeOfOperation(EcCia402Data::Cia402Mode mode)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::setTargetPosition(int32_t targetPosition)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::setTargetVelocity(int32_t targetVelocity)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::setTargetTorque(int16_t targetTorque)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::syncPosition()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::syncVelocity()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcCia402::syncTorque()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}
