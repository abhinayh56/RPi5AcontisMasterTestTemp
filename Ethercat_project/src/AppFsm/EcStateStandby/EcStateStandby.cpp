#include "EcStateStandby.h"

EcStateStandbyNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateStandbyNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::callback()
{
    std::cout << "STANDBY ENTRY" << std::endl;
    return 0;
}

EcStateStandbyNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateStandbyNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateStandbyNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnExitSubroutine::callback()
{
    std::cout << "STANDBY EXIT" << std::endl;
    return 0;
}

EcStateStandbyNs::CallbackSubroutine::CallbackSubroutine() :
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

EcStateStandbyNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateStandbyNs::CallbackSubroutine::config()
{
    m_InputCh_1.subscribe();
    m_InputCh_2.subscribe();
    m_InputCh_3.subscribe();
    m_InputCh_4.subscribe();
    m_InputCh_5.subscribe();
    m_InputCh_6.subscribe();
    m_InputCh_7.subscribe();
    m_InputCh_8.subscribe();

    return 0;
}

uint32_t EcStateStandbyNs::CallbackSubroutine::callback()
{
    m_InputCh_1.get(m_data_1);
    m_InputCh_2.get(m_data_2);
    m_InputCh_3.get(m_data_3);
    m_InputCh_4.get(m_data_4);
    m_InputCh_5.get(m_data_5);
    m_InputCh_6.get(m_data_6);
    m_InputCh_7.get(m_data_7);
    m_InputCh_8.get(m_data_8);

    if(m_data_1 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->emergencyStop();   std::cout << "---\nemergencyStop   : "<< out << std::endl;}
    if(m_data_2 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->faultClear();      std::cout << "---\nclearFault      : "<< out << std::endl;}
    if(m_data_3 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->disable();         std::cout << "---\ndisable         : "<< out << std::endl;}
    if(m_data_4 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModePosition(); std::cout << "---\nsetModePosition : "<< out << std::endl;}
    if(m_data_5 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeVelocity(); std::cout << "---\nsetModeVelocity : "<< out << std::endl;}
    if(m_data_6 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeTorque();   std::cout << "---\nsetModeTorque   : "<< out << std::endl;}
    // if(m_data_7 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->enable();          std::cout << "---\nenable          : "<< out << std::endl;}
    if(m_data_8 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->quickStop();       std::cout << "---\nquickStop       : "<< out << std::endl;}

    ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();

    std::cout << "STANDBY CALLBACK" << std::endl;

    return 0;
}

EcStateStandbyNs::TransitionSubroutine::TransitionSubroutine() :
    m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true)
{
}

EcStateStandbyNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateStandbyNs::TransitionSubroutine::config()
{
    m_InputCh_7.subscribe();
    return 0;
}

uint32_t EcStateStandbyNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    m_InputCh_7.get(m_data_7);

    // if(ecTaskAll.p_ecTaskEthercatSlaveServo->checkFault() != 0)
    // {
    //     std::cout << "STANDBY TRANSITION" << std::endl;
    // 	nextStateId = EcStateData::StateId::FAULT;
    // }
    // else 
    if(m_data_7 == true)
    {
        std::cout << "STANDBY TRANSITION" << std::endl;
        nextStateId = EcStateData::StateId::ENABLING;
    }
    else
    {
    	nextStateId = EcStateData::StateId::STANDBY;
    }

    return CallbackStatus::SUCCESS;
}
