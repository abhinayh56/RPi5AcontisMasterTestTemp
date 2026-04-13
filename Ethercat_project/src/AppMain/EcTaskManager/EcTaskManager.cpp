#include "EcTaskManager.h"

EcTaskManager::EcTaskManager() :
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

EcTaskManager::~EcTaskManager()
{
}

EC_T_DWORD EcTaskManager::initTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= ecTaskInterface.addAllTaskInterface();
    dwRes |= ecTaskEthercatSlave.addAllTaskSlave();   
    dwRes |= ecTaskEthercatSlaveServo.addAllTaskSlave();

    dwRes |= ecTaskRobotControl.addAllTaskRobotControl();
    dwRes |= ecTaskUser.addAllTaskUser();

    return dwRes;
}

EC_T_DWORD EcTaskManager::preapareTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecTaskEthercatSlave.checkSlave();
    dwRes |= ecTaskEthercatSlaveServo.checkSlave();

	dwRes |= ecTaskEthercatSlave.registerPdo();
    dwRes |= ecTaskEthercatSlaveServo.registerPdo();

    dwRes |= ecTaskInterface.registerPublisher();
	dwRes |= ecTaskEthercatSlave.registerPublisher();
    dwRes |= ecTaskEthercatSlaveServo.registerPublisher();

	dwRes |= ecTaskUser.registerPublisher();
    dwRes |= ecTaskRobotControl.registerPublisher();
    dwRes |= ecTaskInterface.registerSubscriber();
	dwRes |= ecTaskEthercatSlave.registerSubscriber();
    dwRes |= ecTaskEthercatSlaveServo.registerSubscriber();

	dwRes |= ecTaskUser.registerSubscriber();
    dwRes |= ecTaskRobotControl.registerSubscriber();

    init_fsm();

    return dwRes;
}

EC_T_DWORD EcTaskManager::setupTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskManager::cyclicTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecTaskEthercatSlave.transferTxPdo();
    dwRes |= ecTaskEthercatSlaveServo.transferTxPdo();

//	ecTaskEthercatSlave.dispTxPdo();
	dwRes |= ecTaskEthercatSlave.processTxPdo();
    dwRes |= ecTaskEthercatSlaveServo.processTxPdo();

	dwRes |= ecTaskEthercatSlave.publishData();
    dwRes |= ecTaskEthercatSlaveServo.publishData();


    dwRes |= ecTaskInterface.subscribeData();

    update_fsm();

    dwRes |= ecTaskUser.subscribeData();
    dwRes |= ecTaskUser.mainProcess();
    dwRes |= ecTaskUser.publishData();

    dwRes |= ecTaskRobotControl.subscribeData();
    dwRes |= ecTaskRobotControl.mainProcess();
    dwRes |= ecTaskRobotControl.publishData();

    dwRes |= ecTaskInterface.publishData();

	dwRes |= ecTaskEthercatSlave.subscribeData();
    dwRes |= ecTaskEthercatSlaveServo.subscribeData();

	dwRes |= ecTaskEthercatSlave.mainProcess();
    dwRes |= ecTaskEthercatSlaveServo.mainProcess();

	dwRes |= ecTaskEthercatSlave.processRxPdo();
    dwRes |= ecTaskEthercatSlaveServo.processRxPdo();

//	ecTaskEthercatSlave.dispRxPdo();
	dwRes |= ecTaskEthercatSlave.transferRxPdo();
    dwRes |= ecTaskEthercatSlaveServo.transferRxPdo();


    return dwRes;
}

EC_T_DWORD EcTaskManager::diagnosisTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskManager::notifyTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

void EcTaskManager::init_fsm()
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

void EcTaskManager::update_fsm()
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

    if(m_data_1 == true) {uint16_t out = ecTaskEthercatSlaveServo.emergencyStop();   std::cout << "---\nemergencyStop   : "<< out << std::endl;}
	if(m_data_2 == true) {uint16_t out = ecTaskEthercatSlaveServo.clearFault();      std::cout << "---\nclearFault      : "<< out << std::endl;}
	if(m_data_3 == true) {uint16_t out = ecTaskEthercatSlaveServo.disable();         std::cout << "---\ndisable         : "<< out << std::endl;}
	if(m_data_4 == true) {uint16_t out = ecTaskEthercatSlaveServo.setModePosition(); std::cout << "---\nsetModePosition : "<< out << std::endl;}
	if(m_data_5 == true) {uint16_t out = ecTaskEthercatSlaveServo.setModeVelocity(); std::cout << "---\nsetModeVelocity : "<< out << std::endl;}
	if(m_data_6 == true) {uint16_t out = ecTaskEthercatSlaveServo.setModeTorque();   std::cout << "---\nsetModeTorque   : "<< out << std::endl;}
	if(m_data_7 == true) {uint16_t out = ecTaskEthercatSlaveServo.enable();          std::cout << "---\nenable          : "<< out << std::endl;}
	if(m_data_8 == true) {uint16_t out = ecTaskEthercatSlaveServo.quickStop();       std::cout << "---\nquickStop       : "<< out << std::endl;}

    ecTaskEthercatSlaveServo.syncPosition();
}