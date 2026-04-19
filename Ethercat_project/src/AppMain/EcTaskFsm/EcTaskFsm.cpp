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

    m_stateVector.push_back(new EcTaskStateBase("STANDBY", EcStateData::StateId::STANDBY));
    m_stateVector.push_back(new EcTaskStateBase("FAULT", EcStateData::StateId::FAULT));
    m_stateVector.push_back(new EcTaskStateBase("CLEARING_FAULT", EcStateData::StateId::CLEARING_FAULT));
    m_stateVector.push_back(new EcTaskStateBase("INITIALIZING", EcStateData::StateId::INITIALIZING));
    m_stateVector.push_back(new EcTaskStateBase("INITIALIZED", EcStateData::StateId::INITIALIZED));
    m_stateVector.push_back(new EcTaskStateBase("DISABLING", EcStateData::StateId::DISABLING));
    m_stateVector.push_back(new EcTaskStateBase("DISABLED", EcStateData::StateId::DISABLED));
    m_stateVector.push_back(new EcTaskStateBase("ENABLING",  EcStateData::StateId::ENABLING));
    m_stateVector.push_back(new EcTaskStateBase("ENABLED", EcStateData::StateId::ENABLED));
    m_stateVector.push_back(new EcTaskStateBase("READY", EcStateData::StateId::READY));
    m_stateVector.push_back(new EcTaskStateBase("JOYSTICKCONTROL",  EcStateData::StateId::JOYSTICKCONTROL));
    
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

    m_stateVector[0]->setTaskAddr(ecTaskAll);
    m_stateVector[0]->addOnEntry(new EcStateStandbyNs::OnEntrySubroutine());
    m_stateVector[0]->addOnExit(new EcStateStandbyNs::OnExitSubroutine());
    m_stateVector[0]->addCallback(new EcStateStandbyNs::CallbackSubroutine());
    m_stateVector[0]->addTransition(new EcStateStandbyNs::TransitionSubroutine());

    m_stateVector[1]->setTaskAddr(ecTaskAll);
    m_stateVector[1]->addOnEntry(new EcStateFaultNs::OnEntrySubroutine());
    m_stateVector[1]->addOnExit(new EcStateFaultNs::OnExitSubroutine());
    m_stateVector[1]->addCallback(new EcStateFaultNs::CallbackSubroutine());
    m_stateVector[1]->addTransition(new EcStateFaultNs::TransitionSubroutine());

    m_stateVector[2]->setTaskAddr(ecTaskAll);
    m_stateVector[2]->addOnEntry(new EcStateClearingFaultNs::OnEntrySubroutine());
    m_stateVector[2]->addOnExit(new EcStateClearingFaultNs::OnExitSubroutine());
    m_stateVector[2]->addCallback(new EcStateClearingFaultNs::CallbackSubroutine());
    m_stateVector[2]->addTransition(new EcStateClearingFaultNs::TransitionSubroutine());

    m_stateVector[3]->setTaskAddr(ecTaskAll);
    m_stateVector[3]->addOnEntry(new EcStateInitializingNs::OnEntrySubroutine());
    m_stateVector[3]->addOnExit(new EcStateInitializingNs::OnExitSubroutine());
    m_stateVector[3]->addCallback(new EcStateInitializingNs::CallbackSubroutine());
    m_stateVector[3]->addTransition(new EcStateInitializingNs::TransitionSubroutine());

    m_stateVector[4]->setTaskAddr(ecTaskAll);
    m_stateVector[4]->addOnEntry(new EcStateInitializedNs::OnEntrySubroutine());
    m_stateVector[4]->addOnExit(new EcStateInitializedNs::OnExitSubroutine());
    m_stateVector[4]->addCallback(new EcStateInitializedNs::CallbackSubroutine());
    m_stateVector[4]->addTransition(new EcStateInitializedNs::TransitionSubroutine());

    m_stateVector[5]->setTaskAddr(ecTaskAll);
    m_stateVector[5]->addOnEntry(new EcStateDisablingNs::OnEntrySubroutine());
    m_stateVector[5]->addOnExit(new EcStateDisablingNs::OnExitSubroutine());
    m_stateVector[5]->addCallback(new EcStateDisablingNs::CallbackSubroutine());
    m_stateVector[5]->addTransition(new EcStateDisablingNs::TransitionSubroutine());

    m_stateVector[6]->setTaskAddr(ecTaskAll);
    m_stateVector[6]->addOnEntry(new EcStateDisabledNs::OnEntrySubroutine());
    m_stateVector[6]->addOnExit(new EcStateDisabledNs::OnExitSubroutine());
    m_stateVector[6]->addCallback(new EcStateDisabledNs::CallbackSubroutine());
    m_stateVector[6]->addTransition(new EcStateDisabledNs::TransitionSubroutine());

    m_stateVector[7]->setTaskAddr(ecTaskAll);
    m_stateVector[7]->addOnEntry(new EcStateEnablingNs::OnEntrySubroutine());
    m_stateVector[7]->addOnExit(new EcStateEnablingNs::OnExitSubroutine());
    m_stateVector[7]->addCallback(new EcStateEnablingNs::CallbackSubroutine());
    m_stateVector[7]->addTransition(new EcStateEnablingNs::TransitionSubroutine());

    m_stateVector[8]->setTaskAddr(ecTaskAll);
    m_stateVector[8]->addOnEntry(new EcStateEnabledNs::OnEntrySubroutine());
    m_stateVector[8]->addOnExit(new EcStateEnabledNs::OnExitSubroutine());
    m_stateVector[8]->addCallback(new EcStateEnabledNs::CallbackSubroutine());
    m_stateVector[8]->addTransition(new EcStateEnabledNs::TransitionSubroutine());

    m_stateVector[9]->setTaskAddr(ecTaskAll);
    m_stateVector[9]->addOnEntry(new EcStateReadyNs::OnEntrySubroutine());
    m_stateVector[9]->addOnExit(new EcStateReadyNs::OnExitSubroutine());
    m_stateVector[9]->addCallback(new EcStateReadyNs::CallbackSubroutine());
    m_stateVector[9]->addTransition(new EcStateReadyNs::TransitionSubroutine());

    m_stateVector[10]->setTaskAddr(ecTaskAll);
    m_stateVector[10]->addOnEntry(new EcStateJoystickControlNs::OnEntrySubroutine());
    m_stateVector[10]->addOnExit(new EcStateJoystickControlNs::OnExitSubroutine());
    m_stateVector[10]->addCallback(new EcStateJoystickControlNs::CallbackSubroutine());
    m_stateVector[10]->addTransition(new EcStateJoystickControlNs::TransitionSubroutine());

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
