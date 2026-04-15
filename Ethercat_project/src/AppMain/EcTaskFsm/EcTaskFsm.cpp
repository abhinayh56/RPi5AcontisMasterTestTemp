#include "EcTaskFsm.h"

EcTaskFsm::EcTaskFsm() :
    p_ecTaskEthercatSlave(nullptr),
    p_ecTaskEthercatSlaveServo(nullptr),
    p_ecTaskRobotControl(nullptr),
    p_ecTaskUser(nullptr),
    p_ecTaskInterface(nullptr),
    m_ecStateStandby("STANDBY", 101),
    m_ecStateFault("FAULT", 102),
    m_ecStateClearingFault("CLEARING_FAULT", 103),
    m_ecStateInitializing("INITIALIZING", 104),
    m_ecStateInitialized("INITIALIZED", 105),
    m_ecStateReady("READY", 106),
    m_ecStateDisabling("DISABLING", 107),
    m_ecStateDisabled("DISABLED", 108),
    m_ecStateEnabled("ENABLED", 109),
    m_ecStateEnabling("ENABLING",  100),
    m_ecStateJoystickControl("JOYSTICKCONTROL",  101),
    m_fsm(101),
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

    p_ecTaskEthercatSlave = p_ecTaskEthercatSlave_;
    p_ecTaskEthercatSlaveServo = p_ecTaskEthercatSlaveServo_;
    p_ecTaskRobotControl = p_ecTaskRobotControl_;
    p_ecTaskUser = p_ecTaskUser_;
    p_ecTaskInterface = p_ecTaskInterface_;

    return dwRes;
}

uint32_t EcTaskFsm::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    init_fsm();

    return dwRes;
}

uint32_t EcTaskFsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

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

    if(m_data_1 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->emergencyStop();   std::cout << "---\nemergencyStop   : "<< out << std::endl;}
	if(m_data_2 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->clearFault();      std::cout << "---\nclearFault      : "<< out << std::endl;}
	if(m_data_3 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->disable();         std::cout << "---\ndisable         : "<< out << std::endl;}
	if(m_data_4 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->setModePosition(); std::cout << "---\nsetModePosition : "<< out << std::endl;}
	if(m_data_5 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->setModeVelocity(); std::cout << "---\nsetModeVelocity : "<< out << std::endl;}
	if(m_data_6 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->setModeTorque();   std::cout << "---\nsetModeTorque   : "<< out << std::endl;}
	if(m_data_7 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->enable();          std::cout << "---\nenable          : "<< out << std::endl;}
	if(m_data_8 == true) {uint16_t out = p_ecTaskEthercatSlaveServo->quickStop();       std::cout << "---\nquickStop       : "<< out << std::endl;}

    p_ecTaskEthercatSlaveServo->syncPosition();
}
