#include "EcStateDisabled.h"

EcStateDisabled::EcStateDisabled() : EcTaskStateBase("STANDBY", EcStateData::StateId::STANDBY)
{
}

EcStateDisabled::~EcStateDisabled()
{
}

uint32_t EcStateDisabled::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateDisabledNs::OnEntrySubroutine());
    addOnExit(new EcStateDisabledNs::OnExitSubroutine());
    addCallback(new EcStateDisabledNs::CallbackSubroutine());
    addTransition(new EcStateDisabledNs::TransitionSubroutine());

    return dwRes;
}

EcStateDisabledNs::OnEntrySubroutine::OnEntrySubroutine()
{
}

EcStateDisabledNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::callback()
{
    std::cout << "DISABLED ENTRY" << std::endl;
    return 0;
}

EcStateDisabledNs::OnExitSubroutine::OnExitSubroutine()
{
}

EcStateDisabledNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateDisabledNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnExitSubroutine::callback()
{
    std::cout << "DISABLED EXIT" << std::endl;
    return 0;
}

EcStateDisabledNs::CallbackSubroutine::CallbackSubroutine() : m_InputCh_1("DIGITAL_INPUT_CH_1", "/ethercat/el1008", false, true),
                                                              m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true),
                                                              m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/el1008", false, true),
                                                              m_InputCh_4("DIGITAL_INPUT_CH_4", "/ethercat/el1008", false, true),
                                                              m_InputCh_5("DIGITAL_INPUT_CH_5", "/ethercat/el1008", false, true),
                                                              m_InputCh_6("DIGITAL_INPUT_CH_6", "/ethercat/el1008", false, true),
                                                              m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true),
                                                              m_InputCh_8("DIGITAL_INPUT_CH_8", "/ethercat/el1008", false, true)
{
}

EcStateDisabledNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateDisabledNs::CallbackSubroutine::config()
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

uint32_t EcStateDisabledNs::CallbackSubroutine::callback()
{
    std::cout << "DISABLED CALLBACK" << std::endl;

    m_InputCh_1.get(m_data_1);
    m_InputCh_2.get(m_data_2);
    m_InputCh_3.get(m_data_3);
    m_InputCh_4.get(m_data_4);
    m_InputCh_5.get(m_data_5);
    m_InputCh_6.get(m_data_6);
    m_InputCh_7.get(m_data_7);
    m_InputCh_8.get(m_data_8);

    // if(m_data_1 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->emergencyStop();   std::cout << "---\nemergencyStop   : "<< out << std::endl;}
    // if(m_data_2 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->clearFault();      std::cout << "---\nclearFault      : "<< out << std::endl;}
    // if(m_data_3 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->disable();         std::cout << "---\ndisable         : "<< out << std::endl;}
    if (m_data_4 == true)
    {
        uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModePosition();
        std::cout << "---\nsetModePosition : " << out << std::endl;
    }
    if (m_data_5 == true)
    {
        uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeVelocity();
        std::cout << "---\nsetModeVelocity : " << out << std::endl;
    }
    if (m_data_6 == true)
    {
        uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->setModeTorque();
        std::cout << "---\nsetModeTorque   : " << out << std::endl;
    }
    // if(m_data_7 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->enable();          std::cout << "---\nenable          : "<< out << std::endl;}
    // if(m_data_8 == true) {uint16_t out = ecTaskAll.p_ecTaskEthercatSlaveServo->quickStop();       std::cout << "---\nquickStop       : "<< out << std::endl;}

    ecTaskAll.p_ecTaskEthercatSlaveServo->syncPosition();

    return 0;
}

EcStateDisabledNs::TransitionSubroutine::TransitionSubroutine() : m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true)
{
}

EcStateDisabledNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateDisabledNs::TransitionSubroutine::config()
{
    m_InputCh_7.subscribe();
    return 0;
}

uint32_t EcStateDisabledNs::TransitionSubroutine::callback(uint32_t &nextStateId)
{
    m_InputCh_7.get(m_data_7);

    if (ecTaskAll.p_ecTaskEthercatSlaveServo->isFaultClear() == 0)
    {
        std::cout << "DISABLED TRANSITION" << std::endl;
        nextStateId = EcStateData::StateId::FAULT;
    }
    else if (m_data_7 == true)
    {
        std::cout << "DISABLED TRANSITION" << std::endl;
        nextStateId = EcStateData::StateId::ENABLING;
    }
    else
    {
        nextStateId = EcStateData::StateId::DISABLED;
    }

    return CallbackStatus::SUCCESS;
}