#include "EcUser_2.h"

EcUser_2::EcUser_2(uint16_t ecTaskUserId, const std::string &ecTaskUserName) :
    EcTaskUserBase(ecTaskUserId, ecTaskUserName),
    m_InputCh_1("DIGITAL_INPUT_CH_1", "/ethercat/el1008", false, true),
    m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true),
    m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/el1008", false, true),
    m_InputCh_4("DIGITAL_INPUT_CH_4", "/ethercat/el1008", false, true),
    m_InputCh_5("DIGITAL_INPUT_CH_5", "/ethercat/el1008", false, true),
    m_InputCh_6("DIGITAL_INPUT_CH_6", "/ethercat/el1008", false, true),
    m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true),
    m_InputCh_8("DIGITAL_INPUT_CH_8", "/ethercat/el1008", false, true),
    m_OutputCh_1("DIGITAL_OUTPUT_CH_1", "/ethercat/el2008", false, true),
    m_OutputCh_2("DIGITAL_OUTPUT_CH_2", "/ethercat/el2008", false, true),
    m_OutputCh_3("DIGITAL_OUTPUT_CH_3", "/ethercat/el2008", false, true),
    m_OutputCh_4("DIGITAL_OUTPUT_CH_4", "/ethercat/el2008", false, true),
    m_OutputCh_5("DIGITAL_OUTPUT_CH_5", "/ethercat/el2008", false, true),
    m_OutputCh_6("DIGITAL_OUTPUT_CH_6", "/ethercat/el2008", false, true),
    m_OutputCh_7("DIGITAL_OUTPUT_CH_7", "/ethercat/el2008", false, true),
    m_OutputCh_8("DIGITAL_OUTPUT_CH_8", "/ethercat/el2008", false, true)
{
}

EcUser_2::~EcUser_2()
{
}

EC_T_DWORD EcUser_2::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_OutputCh_1.publish();
    m_OutputCh_2.publish();
    m_OutputCh_3.publish();
    m_OutputCh_4.publish();
    m_OutputCh_5.publish();
    m_OutputCh_6.publish();
    m_OutputCh_7.publish();
    m_OutputCh_8.publish();

    return dwRes;
}

EC_T_DWORD EcUser_2::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_InputCh_1.subscribe();
    m_InputCh_2.subscribe();
    m_InputCh_3.subscribe();
    m_InputCh_4.subscribe();
    m_InputCh_5.subscribe();
    m_InputCh_6.subscribe();
    m_InputCh_7.subscribe();
    m_InputCh_8.subscribe();

    return dwRes;
}

EC_T_DWORD EcUser_2::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_OutputCh_1.set(m_data_1);
    m_OutputCh_2.set(m_data_2);
    m_OutputCh_3.set(m_data_3);
    m_OutputCh_4.set(m_data_4);
    m_OutputCh_5.set(m_data_5);
    m_OutputCh_6.set(m_data_6);
    m_OutputCh_7.set(m_data_7);
    m_OutputCh_8.set(m_data_8);

    return dwRes;
}

EC_T_DWORD EcUser_2::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_InputCh_1.get(m_data_1);
    m_InputCh_2.get(m_data_2);
    m_InputCh_3.get(m_data_3);
    m_InputCh_4.get(m_data_4);
    m_InputCh_5.get(m_data_5);
    m_InputCh_6.get(m_data_6);
    m_InputCh_7.get(m_data_7);
    m_InputCh_8.get(m_data_8);

    return dwRes;
}

EC_T_DWORD EcUser_2::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

