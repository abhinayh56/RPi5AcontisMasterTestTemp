#include "EcTaskFsm.h"

EcTaskFsm::EcTaskFsm() :
     ecTaskAll({nullptr, nullptr, nullptr, nullptr, nullptr}),
    // ecTaskAll.p_ecTaskEthercatSlaveServo(nullptr),
    // ecTaskAll.p_ecTaskRobotControl(nullptr),
    // ecTaskAll.p_ecTaskUser(nullptr),
    // ecTaskAll.p_ecTaskInterface(nullptr),
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
    m_fsm(EcStateData::StateId::STANDBY),
    m_InputCh_1("DIGITAL_INPUT_CH_1", "/ethercat/el1008", false, true),
    m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true),
    m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/el1008", false, true),
    m_InputCh_4("DIGITAL_INPUT_CH_4", "/ethercat/el1008", false, true),
    m_InputCh_5("DIGITAL_INPUT_CH_5", "/ethercat/el1008", false, true),
    m_InputCh_6("DIGITAL_INPUT_CH_6", "/ethercat/el1008", false, true),
    m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true),
    m_InputCh_8("DIGITAL_INPUT_CH_8", "/ethercat/el1008", false, true)
{
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

uint32_t EcTaskFsm::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_ecStateStandby.addOnEntry(new EcStateStandbyNs::OnEntrySubroutine());
    m_ecStateStandby.addOnExit(new EcStateStandbyNs::OnExitSubroutine());
    m_ecStateStandby.addCallback(new EcStateStandbyNs::CallbackSubroutine());
    m_ecStateStandby.addTransition(new EcStateStandbyNs::TransitionSubroutine());

    m_ecStateFault.addOnEntry(new EcStateFaultNs::OnEntrySubroutine());
    m_ecStateFault.addOnExit(new EcStateFaultNs::OnExitSubroutine());
    m_ecStateFault.addCallback(new EcStateFaultNs::CallbackSubroutine());
    m_ecStateFault.addTransition(new EcStateFaultNs::TransitionSubroutine());

    m_ecStateClearingFault.addOnEntry(new EcStateClearingFaultNs::OnEntrySubroutine());
    m_ecStateClearingFault.addOnExit(new EcStateClearingFaultNs::OnExitSubroutine());
    m_ecStateClearingFault.addCallback(new EcStateClearingFaultNs::CallbackSubroutine());
    m_ecStateClearingFault.addTransition(new EcStateClearingFaultNs::TransitionSubroutine());

    m_ecStateInitializing.addOnEntry(new EcStateInitializingNs::OnEntrySubroutine());
    m_ecStateInitializing.addOnExit(new EcStateInitializingNs::OnExitSubroutine());
    m_ecStateInitializing.addCallback(new EcStateInitializingNs::CallbackSubroutine());
    m_ecStateInitializing.addTransition(new EcStateInitializingNs::TransitionSubroutine());

    m_ecStateInitialized.addOnEntry(new EcStateInitializedNs::OnEntrySubroutine());
    m_ecStateInitialized.addOnExit(new EcStateInitializedNs::OnExitSubroutine());
    m_ecStateInitialized.addCallback(new EcStateInitializedNs::CallbackSubroutine());
    m_ecStateInitialized.addTransition(new EcStateInitializedNs::TransitionSubroutine());

    m_ecStateDisabling.addOnEntry(new EcStateDisablingNs::OnEntrySubroutine());
    m_ecStateDisabling.addOnExit(new EcStateDisablingNs::OnExitSubroutine());
    m_ecStateDisabling.addCallback(new EcStateDisablingNs::CallbackSubroutine());
    m_ecStateDisabling.addTransition(new EcStateDisablingNs::TransitionSubroutine());

    m_ecStateDisabled.addOnEntry(new EcStateDisabledNs::OnEntrySubroutine());
    m_ecStateDisabled.addOnExit(new EcStateDisabledNs::OnExitSubroutine());
    m_ecStateDisabled.addCallback(new EcStateDisabledNs::CallbackSubroutine());
    m_ecStateDisabled.addTransition(new EcStateDisabledNs::TransitionSubroutine());

    m_ecStateEnabling.addOnEntry(new EcStateEnablingNs::OnEntrySubroutine());
    m_ecStateEnabling.addOnExit(new EcStateEnablingNs::OnExitSubroutine());
    m_ecStateEnabling.addCallback(new EcStateEnablingNs::CallbackSubroutine());
    m_ecStateEnabling.addTransition(new EcStateEnablingNs::TransitionSubroutine());

    m_ecStateEnabled.addOnEntry(new EcStateEnabledNs::OnEntrySubroutine());
    m_ecStateEnabled.addOnExit(new EcStateEnabledNs::OnExitSubroutine());
    m_ecStateEnabled.addCallback(new EcStateEnabledNs::CallbackSubroutine());
    m_ecStateEnabled.addTransition(new EcStateEnabledNs::TransitionSubroutine());

    m_ecStateReady.addOnEntry(new EcStateReadyNs::OnEntrySubroutine());
    m_ecStateReady.addOnExit(new EcStateReadyNs::OnExitSubroutine());
    m_ecStateReady.addCallback(new EcStateReadyNs::CallbackSubroutine());
    m_ecStateReady.addTransition(new EcStateReadyNs::TransitionSubroutine());

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

    init_fsm();

    return dwRes;
}

uint32_t EcTaskFsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.update();

    update_fsm();

    return dwRes;
}

void EcTaskFsm::init_fsm()
{
    m_InputCh_1.subscribe();
    m_InputCh_2.subscribe();
    m_InputCh_3.subscribe();
    m_InputCh_4.subscribe();
    m_InputCh_5.subscribe();
    m_InputCh_6.subscribe();
    m_InputCh_7.subscribe();
    m_InputCh_8.subscribe();
}

void EcTaskFsm::update_fsm()
{
    m_InputCh_1.get(m_data_1);
    m_InputCh_2.get(m_data_2);
    m_InputCh_3.get(m_data_3);
    m_InputCh_4.get(m_data_4);
    m_InputCh_5.get(m_data_5);
    m_InputCh_6.get(m_data_6);
    m_InputCh_7.get(m_data_7);
    m_InputCh_8.get(m_data_8);

    // std::cout << "fsm_input: " <<
    // m_data_1 << ", " <<
    // m_data_2 << ", " <<
    // m_data_3 << ", " <<
    // m_data_4 << ", " <<
    // m_data_5 << ", " <<
    // m_data_6 << ", " <<
    // m_data_7 << ", " <<
    // m_data_8 << std::endl;

    if(m_data_1 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->emergencyStop();   std::cout << "---\nemergencyStop   : "<< out << std::endl;}
	if(m_data_2 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->clearFault();      std::cout << "---\nclearFault      : "<< out << std::endl;}
	if(m_data_3 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->disable();         std::cout << "---\ndisable         : "<< out << std::endl;}
	if(m_data_4 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModePosition(); std::cout << "---\nsetModePosition : "<< out << std::endl;}
	if(m_data_5 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeVelocity(); std::cout << "---\nsetModeVelocity : "<< out << std::endl;}
	if(m_data_6 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeTorque();   std::cout << "---\nsetModeTorque   : "<< out << std::endl;}
	if(m_data_7 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->enable();          std::cout << "---\nenable          : "<< out << std::endl;}
	if(m_data_8 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->quickStop();       std::cout << "---\nquickStop       : "<< out << std::endl;}

    ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();
}
