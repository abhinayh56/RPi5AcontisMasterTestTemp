#include "EcTaskFsm.h"

EcTaskFsm::EcTaskFsm() :
     ecTaskAll({nullptr, nullptr, nullptr, nullptr, nullptr}),
    m_ecStateStandby("STANDBY", EcStateData::StateId::STANDBY),
    m_ecStateFault("FAULT", EcStateData::StateId::FAULT),
    m_ecStateClearingFault("CLEARING_FAULT", EcStateData::StateId::CLEARING_FAULT),
    m_ecStateInitializing("INITIALIZING", EcStateData::StateId::INITIALIZING),
    m_ecStateInitialized("INITIALIZED", EcStateData::StateId::INITIALIZED),
    m_ecStateDisabling("DISABLING", EcStateData::StateId::DISABLING),
    m_ecStateDisabled("DISABLED", EcStateData::StateId::DISABLED),
    m_ecStateEnabling("ENABLING",  EcStateData::StateId::ENABLING),
    m_ecStateEnabled("ENABLED", EcStateData::StateId::ENABLED),
    m_ecStateReady("READY", EcStateData::StateId::READY),
    m_ecStateJoystickControl("JOYSTICKCONTROL",  EcStateData::StateId::JOYSTICKCONTROL),
    m_fsm(EcStateData::StateId::STANDBY)
{
    m_stateVector.reserve(25);
}

EcTaskFsm::~EcTaskFsm()
{
}

uint32_t EcTaskFsm::setTaskAddr(
        EcTaskEthercatSlave* p_ecTaskEthercatSlave_,
        EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo_,
        EcTaskRobotControl* p_ecTaskRobotControl_,
        EcTaskUser* p_ecTaskUser_,
        EcTaskInterface* p_ecTaskInterface_
    )
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    ecTaskAll.p_ecTaskEthercatSlave = p_ecTaskEthercatSlave_;
    ecTaskAll.p_ecTaskEthercatSlaveServo = p_ecTaskEthercatSlaveServo_;
    ecTaskAll.p_ecTaskRobotControl = p_ecTaskRobotControl_;
    ecTaskAll.p_ecTaskUser = p_ecTaskUser_;
    ecTaskAll.p_ecTaskInterface = p_ecTaskInterface_;

    return dwRes;
}

EC_T_DWORD EcTaskFsm::addAllState()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_stateVector.push_back(new State("STANDBY", EcStateData::StateId::STANDBY));
    m_stateVector.push_back(new State("FAULT", EcStateData::StateId::FAULT));
    m_stateVector.push_back(new State("CLEARING_FAULT", EcStateData::StateId::CLEARING_FAULT));
    m_stateVector.push_back(new State("INITIALIZING", EcStateData::StateId::INITIALIZING));
    m_stateVector.push_back(new State("INITIALIZED", EcStateData::StateId::INITIALIZED));
    m_stateVector.push_back(new State("DISABLING", EcStateData::StateId::DISABLING));
    m_stateVector.push_back(new State("DISABLED", EcStateData::StateId::DISABLED));
    m_stateVector.push_back(new State("ENABLING",  EcStateData::StateId::ENABLING));
    m_stateVector.push_back(new State("ENABLED", EcStateData::StateId::ENABLED));
    m_stateVector.push_back(new State("READY", EcStateData::StateId::READY));
    m_stateVector.push_back(new State("JOYSTICKCONTROL",  EcStateData::StateId::JOYSTICKCONTROL));
    
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

    m_ecStateStandby.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateStandby.addOnEntry(new EcStateStandbyNs::OnEntrySubroutine());
    m_ecStateStandby.addOnExit(new EcStateStandbyNs::OnExitSubroutine());
    m_ecStateStandby.addCallback(new EcStateStandbyNs::CallbackSubroutine());
    m_ecStateStandby.addTransition(new EcStateStandbyNs::TransitionSubroutine());

    m_ecStateFault.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateFault.addOnEntry(new EcStateFaultNs::OnEntrySubroutine());
    m_ecStateFault.addOnExit(new EcStateFaultNs::OnExitSubroutine());
    m_ecStateFault.addCallback(new EcStateFaultNs::CallbackSubroutine());
    m_ecStateFault.addTransition(new EcStateFaultNs::TransitionSubroutine());

    m_ecStateClearingFault.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateClearingFault.addOnEntry(new EcStateClearingFaultNs::OnEntrySubroutine());
    m_ecStateClearingFault.addOnExit(new EcStateClearingFaultNs::OnExitSubroutine());
    m_ecStateClearingFault.addCallback(new EcStateClearingFaultNs::CallbackSubroutine());
    m_ecStateClearingFault.addTransition(new EcStateClearingFaultNs::TransitionSubroutine());

    m_ecStateInitializing.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateInitializing.addOnEntry(new EcStateInitializingNs::OnEntrySubroutine());
    m_ecStateInitializing.addOnExit(new EcStateInitializingNs::OnExitSubroutine());
    m_ecStateInitializing.addCallback(new EcStateInitializingNs::CallbackSubroutine());
    m_ecStateInitializing.addTransition(new EcStateInitializingNs::TransitionSubroutine());

    m_ecStateInitialized.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateInitialized.addOnEntry(new EcStateInitializedNs::OnEntrySubroutine());
    m_ecStateInitialized.addOnExit(new EcStateInitializedNs::OnExitSubroutine());
    m_ecStateInitialized.addCallback(new EcStateInitializedNs::CallbackSubroutine());
    m_ecStateInitialized.addTransition(new EcStateInitializedNs::TransitionSubroutine());

    m_ecStateDisabling.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateDisabling.addOnEntry(new EcStateDisablingNs::OnEntrySubroutine());
    m_ecStateDisabling.addOnExit(new EcStateDisablingNs::OnExitSubroutine());
    m_ecStateDisabling.addCallback(new EcStateDisablingNs::CallbackSubroutine());
    m_ecStateDisabling.addTransition(new EcStateDisablingNs::TransitionSubroutine());

    m_ecStateDisabled.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateDisabled.addOnEntry(new EcStateDisabledNs::OnEntrySubroutine());
    m_ecStateDisabled.addOnExit(new EcStateDisabledNs::OnExitSubroutine());
    m_ecStateDisabled.addCallback(new EcStateDisabledNs::CallbackSubroutine());
    m_ecStateDisabled.addTransition(new EcStateDisabledNs::TransitionSubroutine());

    m_ecStateEnabling.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateEnabling.addOnEntry(new EcStateEnablingNs::OnEntrySubroutine());
    m_ecStateEnabling.addOnExit(new EcStateEnablingNs::OnExitSubroutine());
    m_ecStateEnabling.addCallback(new EcStateEnablingNs::CallbackSubroutine());
    m_ecStateEnabling.addTransition(new EcStateEnablingNs::TransitionSubroutine());

    m_ecStateEnabled.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateEnabled.addOnEntry(new EcStateEnabledNs::OnEntrySubroutine());
    m_ecStateEnabled.addOnExit(new EcStateEnabledNs::OnExitSubroutine());
    m_ecStateEnabled.addCallback(new EcStateEnabledNs::CallbackSubroutine());
    m_ecStateEnabled.addTransition(new EcStateEnabledNs::TransitionSubroutine());

    m_ecStateReady.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateReady.addOnEntry(new EcStateReadyNs::OnEntrySubroutine());
    m_ecStateReady.addOnExit(new EcStateReadyNs::OnExitSubroutine());
    m_ecStateReady.addCallback(new EcStateReadyNs::CallbackSubroutine());
    m_ecStateReady.addTransition(new EcStateReadyNs::TransitionSubroutine());

    m_ecStateJoystickControl.setTaskAddr(ecTaskAll.p_ecTaskEthercatSlave,
        ecTaskAll.p_ecTaskEthercatSlaveServo,
        ecTaskAll.p_ecTaskRobotControl,
        ecTaskAll.p_ecTaskUser,
        ecTaskAll.p_ecTaskInterface);
    m_ecStateJoystickControl.addOnEntry(new EcStateJoystickControlNs::OnEntrySubroutine());
    m_ecStateJoystickControl.addOnExit(new EcStateJoystickControlNs::OnExitSubroutine());
    m_ecStateJoystickControl.addCallback(new EcStateJoystickControlNs::CallbackSubroutine());
    m_ecStateJoystickControl.addTransition(new EcStateJoystickControlNs::TransitionSubroutine());

    m_fsm.addState(&m_ecStateStandby);
    m_fsm.addState(&m_ecStateFault);
    m_fsm.addState(&m_ecStateClearingFault);
    m_fsm.addState(&m_ecStateInitializing);
	m_fsm.addState(&m_ecStateInitialized);
	m_fsm.addState(&m_ecStateDisabling);
	m_fsm.addState(&m_ecStateDisabled);
	m_fsm.addState(&m_ecStateEnabling);
	m_fsm.addState(&m_ecStateEnabled);
    m_fsm.addState(&m_ecStateReady);
    m_fsm.addState(&m_ecStateJoystickControl);

    m_fsm.config();

    return dwRes;
}

uint32_t EcTaskFsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.update();

    return dwRes;
}
