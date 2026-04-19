#include "EcTaskFsm.h"

EcTaskFsm::EcTaskFsm() :
     ecTaskAll({nullptr, nullptr, nullptr, nullptr, nullptr}),
	 m_fsm(EcStateData::StateId::STANDBY)
{
    m_stateVector.reserve(25);
}

EcTaskFsm::~EcTaskFsm()
{
}

uint32_t EcTaskFsm::setTaskAddr(EcStateData::EcTaskAll ecTaskAll_)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll = ecTaskAll_;

    return dwRes;
}

EC_T_DWORD EcTaskFsm::addAllState()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_stateVector.push_back(new EcStateStandby());
    m_stateVector.push_back(new EcStateFault());
    m_stateVector.push_back(new EcStateClearingFault());
    m_stateVector.push_back(new EcStateInitializing());
    m_stateVector.push_back(new EcStateInitialized());
    m_stateVector.push_back(new EcStateReady());
    m_stateVector.push_back(new EcStateDisabling());
    m_stateVector.push_back(new EcStateDisabled());
    m_stateVector.push_back(new EcStateEnabled());
    m_stateVector.push_back(new EcStateEnabling());
    m_stateVector.push_back(new EcStateJoystickControl());
    
    return dwRes;
}

EC_T_DWORD EcTaskFsm::cleanupState()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskFsm::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i< m_stateVector.size(); i++)
    {
        m_stateVector[i]->setTaskAddr(ecTaskAll);
        m_stateVector[i]->addSubroutine();
    }

    for(uint32_t i = 0; i<m_stateVector.size(); i++)
    {
        m_fsm.addState(m_stateVector[i]);
    }

    m_fsm.config();

    return dwRes;
}

uint32_t EcTaskFsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.update();

    return dwRes;
}
