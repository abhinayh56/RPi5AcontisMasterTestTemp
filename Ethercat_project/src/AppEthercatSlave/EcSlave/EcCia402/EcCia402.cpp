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

    if
    (    
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::FAULT_REACTION_ACTIVE) ||
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT) == EcCia402Data::Object::StatusWord::BitData::FAULT)
    )
    {
        std::cout << m_slaveName << " : " << "FAULT DETECTED\n";
        dwRes = 1 << 10;
    }
    else
    {
        std::cout << m_slaveName << " : " << "NO FAULT\n";
    }

    return dwRes;
}

EC_T_DWORD EcCia402::clearFault()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    if((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT) == EcCia402Data::Object::StatusWord::BitData::FAULT)
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::FAULT_RESET;
    }

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT) == EcCia402Data::Object::StatusWord::BitData::FAULT) ||
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::FAULT_REACTION_ACTIVE)
    )
    {
        std::cout << "CLEARING FAULT: " << m_slaveName << std::endl;
    }
    else
    {
        std::cout << "FAULT CLEARED : " << m_slaveName << std::endl;
    }

    return dwRes;
}

EC_T_DWORD EcCia402::enable()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::BitData::OPERATION_ENABLED)
    )
    {
        std::cout << ".\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::FAULT_REACTION_ACTIVE)
    )
    {
        std::cout << "14\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT) == EcCia402Data::Object::StatusWord::BitData::FAULT)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::FAULT_RESET;
        std::cout << "15\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::QUICK_STOP_ACTIVE)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::ENABLE_OPERATION;
        std::cout << "16\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::BitData::SWITCHED_ON)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::ENABLE_OPERATION;
        std::cout << "4\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::BitData::READY_TO_SWITCH_ON)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::SWITCH_ON;
        std::cout << "3\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::BitData::SWITCH_ON_DISABLED)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::SHUT_DOWN;
        std::cout << "2\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::NOT_READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::BitData::NOT_READY_TO_SWITCH_ON)
    )
    {
        std::cout << "1\n";
    }

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::BitData::OPERATION_ENABLED)
    )
    {
        std::cout << "ENABLED : " << m_slaveName << std::endl;
    }
    else
    {
        std::cout << "ENABLING: " << m_slaveName << std::endl;
    }

    return dwRes;
}

EC_T_DWORD EcCia402::disable()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;
    
    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::BitData::OPERATION_ENABLED)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::DISABLE_OPERATION;
        std::cout << "5\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::FAULT_REACTION_ACTIVE)
    )
    {
        std::cout << "14\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT) == EcCia402Data::Object::StatusWord::BitData::FAULT)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::FAULT_RESET;
        std::cout << "15\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::QUICK_STOP_ACTIVE)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::DISABLE_VOLTAGE;
        std::cout << "12\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::BitData::SWITCHED_ON)
    )
    {
        std::cout << ".\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::BitData::READY_TO_SWITCH_ON)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::SWITCH_ON;
        std::cout << "3\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::BitData::SWITCH_ON_DISABLED)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::SHUT_DOWN;
        std::cout << "2\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::NOT_READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::BitData::NOT_READY_TO_SWITCH_ON)
    )
    {
        std::cout << "1\n";
    }

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::BitData::SWITCHED_ON)
    )
    {
        std::cout << "DISABLED : " << m_slaveName << std::endl;
    }
    else
    {
        std::cout << "DISABLING: " << m_slaveName << std::endl;
    }

    return dwRes;
}

EC_T_DWORD EcCia402::quickStop()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::QUICK_STOP_ACTIVE)
    )
    {
        std::cout << ".\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::BitData::OPERATION_ENABLED)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::QUICK_STOP;
        std::cout << "11\n";
    }

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::QUICK_STOP_ACTIVE)
    )
    {
        std::cout << "QUICK STOP ACTIVE     : " << m_slaveName << std::endl;
    }
    else
    {
        std::cout << "QUICK STOP IN PROGRESS: " << m_slaveName << std::endl;
    }

    return dwRes;
}

EC_T_DWORD EcCia402::emergencyStop()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::BitData::OPERATION_ENABLED)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::DISABLE_VOLTAGE;
        std::cout << "9\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::FAULT_REACTION_ACTIVE)
    )
    {
        std::cout << "14\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::FAULT) == EcCia402Data::Object::StatusWord::BitData::FAULT)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::FAULT_RESET;
        std::cout << "15\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::BitData::QUICK_STOP_ACTIVE)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::DISABLE_VOLTAGE;
        std::cout << "12\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::BitData::SWITCHED_ON)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::DISABLE_VOLTAGE;
        std::cout << "10\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::BitData::READY_TO_SWITCH_ON)
    )
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::BitData::DISABLE_VOLTAGE;
        std::cout << "7\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::BitData::SWITCH_ON_DISABLED)
    )
    {
        std::cout << ".\n";
    }
    else if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::NOT_READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::BitData::NOT_READY_TO_SWITCH_ON)
    )
    {
        std::cout << "1\n";
    }

    if
    (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::BitData::SWITCH_ON_DISABLED)
    )
    {
        std::cout << "EMERGENCY STOP ACTIVE      : " << m_slaveName << std::endl;
    }
    else
    {
        std::cout << "EMERGENCY STOP IN PROGRESS : " << m_slaveName << std::endl;
    }

    return dwRes;
}

EC_T_DWORD EcCia402::setModePosition()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    setModeOfOperation(EcCia402Data::Object::ModeOfOperation::BitData::CYCLIC_SYNC_POSITION);

    return dwRes;
}

EC_T_DWORD EcCia402::setModeVelocity()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    setModeOfOperation(EcCia402Data::Object::ModeOfOperation::BitData::CYCLIC_SYNC_VELOCITY);

    return dwRes;
}

EC_T_DWORD EcCia402::setModeTorque()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    setModeOfOperation(EcCia402Data::Object::ModeOfOperation::BitData::CYCLIC_SYNC_TORQUE);

    return dwRes;
}

EC_T_DWORD EcCia402::setTargetPosition(int32_t targetPosition)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.targetPosition.p_value = targetPosition;

    return dwRes;
}

EC_T_DWORD EcCia402::setTargetVelocity(int32_t targetVelocity)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.targetVelocity.p_value = targetVelocity;

    return dwRes;
}

EC_T_DWORD EcCia402::setTargetTorque(int16_t targetTorque)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.targetTorque.p_value = targetTorque;

    return dwRes;
}

int32_t EcCia402::getActualPosition()
{
    return *m_Cia402PdoTx.actualPosition.p_value;
}

int32_t EcCia402::getActualVelocity()
{
    return *m_Cia402PdoTx.actualVelocity.p_value;
}

int16_t EcCia402::getActualTorque()
{
    return *m_Cia402PdoTx.actualTorque.p_value;
}

EC_T_DWORD EcCia402::syncPosition()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.targetPosition.p_value = *m_Cia402PdoTx.actualPosition.p_value;

    return dwRes;
}

EC_T_DWORD EcCia402::syncVelocity()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.targetVelocity.p_value = *m_Cia402PdoTx.actualVelocity.p_value;

    return dwRes;
}

EC_T_DWORD EcCia402::syncTorque()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.targetTorque.p_value = *m_Cia402PdoTx.actualTorque.p_value;

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

EC_T_DWORD EcCia402::setModeOfOperation(int8_t mode)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    *m_Cia402PdoRx.modeOfOperation.p_value = mode;

    if(mode==8)
    {
        std::cout << "SETTING CYCLIC_SYNC_POSITION" << std::endl;
    }
    else if(mode==9)
    {
        std::cout << "SETTING CYCLIC_SYNC_VELOCITY" << std::endl;
    }
    else if(mode==10)
    {
        std::cout << "SETTING CYCLIC_SYNC_TORQUE" << std::endl;
    }
    else
    {
        std::cout << "UNKNOWN MODE" << std::endl;
    }

    return dwRes;
}