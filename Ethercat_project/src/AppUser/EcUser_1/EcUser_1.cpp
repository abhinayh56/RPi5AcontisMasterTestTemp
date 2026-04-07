#include "EcUser_1.h"

EcUser_1::EcUser_1(uint16_t ecTaskUserId, const std::string &ecTaskUserName) :
    EcTaskUserBase(ecTaskUserId, ecTaskUserName),
    m_InputCh_1("DIGITAL_INPUT_CH_1", "/ethercat/el1008", false, true),
    m_InputCh_2("DIGITAL_INPUT_CH_2", "/ethercat/el1008", false, true),
    m_InputCh_3("DIGITAL_INPUT_CH_3", "/ethercat/el1008", false, true),
    m_InputCh_4("DIGITAL_INPUT_CH_4", "/ethercat/el1008", false, true),
    m_InputCh_5("DIGITAL_INPUT_CH_5", "/ethercat/el1008", false, true),
    m_InputCh_6("DIGITAL_INPUT_CH_6", "/ethercat/el1008", false, true),
    m_InputCh_7("DIGITAL_INPUT_CH_7", "/ethercat/el1008", false, true),
    m_InputCh_8("DIGITAL_INPUT_CH_8", "/ethercat/el1008", false, true),
    m_OutputCh_11("DATA_1", "/ethercat/motor_1", false, true),
    m_OutputCh_21("DATA_2", "/ethercat/motor_1", false, true),
    m_OutputCh_31("DATA_3", "/ethercat/motor_1", false, true),
    m_OutputCh_41("DATA_4", "/ethercat/motor_1", false, true),
    m_OutputCh_51("DATA_5", "/ethercat/motor_1", false, true),
    m_OutputCh_61("DATA_6", "/ethercat/motor_1", false, true),
    m_OutputCh_71("DATA_7", "/ethercat/motor_1", false, true),
    m_OutputCh_81("DATA_8", "/ethercat/motor_1", false, true),
    m_OutputCh_12("DATA_1", "/ethercat/motor_2", false, true),
    m_OutputCh_22("DATA_2", "/ethercat/motor_2", false, true),
    m_OutputCh_32("DATA_3", "/ethercat/motor_2", false, true),
    m_OutputCh_42("DATA_4", "/ethercat/motor_2", false, true),
    m_OutputCh_52("DATA_5", "/ethercat/motor_2", false, true),
    m_OutputCh_62("DATA_6", "/ethercat/motor_2", false, true),
    m_OutputCh_72("DATA_7", "/ethercat/motor_2", false, true),
    m_OutputCh_82("DATA_8", "/ethercat/motor_2", false, true),
    m_OutputCh_13("DATA_1", "/ethercat/motor_3", false, true),
    m_OutputCh_23("DATA_2", "/ethercat/motor_3", false, true),
    m_OutputCh_33("DATA_3", "/ethercat/motor_3", false, true),
    m_OutputCh_43("DATA_4", "/ethercat/motor_3", false, true),
    m_OutputCh_53("DATA_5", "/ethercat/motor_3", false, true),
    m_OutputCh_63("DATA_6", "/ethercat/motor_3", false, true),
    m_OutputCh_73("DATA_7", "/ethercat/motor_3", false, true),
    m_OutputCh_83("DATA_8", "/ethercat/motor_3", false, true),
    m_OutputCh_14("DATA_1", "/ethercat/motor_4", false, true),
    m_OutputCh_24("DATA_2", "/ethercat/motor_4", false, true),
    m_OutputCh_34("DATA_3", "/ethercat/motor_4", false, true),
    m_OutputCh_44("DATA_4", "/ethercat/motor_4", false, true),
    m_OutputCh_54("DATA_5", "/ethercat/motor_4", false, true),
    m_OutputCh_64("DATA_6", "/ethercat/motor_4", false, true),
    m_OutputCh_74("DATA_7", "/ethercat/motor_4", false, true),
    m_OutputCh_84("DATA_8", "/ethercat/motor_4", false, true)
{
}

EcUser_1::~EcUser_1()
{
}

EC_T_DWORD EcUser_1::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_OutputCh_11.publish();
    m_OutputCh_21.publish();
    m_OutputCh_31.publish();
    m_OutputCh_41.publish();
    m_OutputCh_51.publish();
    m_OutputCh_61.publish();
    m_OutputCh_71.publish();
    m_OutputCh_81.publish();
    m_OutputCh_12.publish();
    m_OutputCh_22.publish();
    m_OutputCh_32.publish();
    m_OutputCh_42.publish();
    m_OutputCh_52.publish();
    m_OutputCh_62.publish();
    m_OutputCh_72.publish();
    m_OutputCh_82.publish();
    m_OutputCh_13.publish();
    m_OutputCh_23.publish();
    m_OutputCh_33.publish();
    m_OutputCh_43.publish();
    m_OutputCh_53.publish();
    m_OutputCh_63.publish();
    m_OutputCh_73.publish();
    m_OutputCh_83.publish();
    m_OutputCh_14.publish();
    m_OutputCh_24.publish();
    m_OutputCh_34.publish();
    m_OutputCh_44.publish();
    m_OutputCh_54.publish();
    m_OutputCh_64.publish();
    m_OutputCh_74.publish();
    m_OutputCh_84.publish();

    return dwRes;
}

EC_T_DWORD EcUser_1::registerSubscriber()
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

EC_T_DWORD EcUser_1::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcUser_1::subscribeData()
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

EC_T_DWORD EcUser_1::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_OutputCh_11.set(m_data_1);
    m_OutputCh_21.set(m_data_2);
    m_OutputCh_31.set(m_data_3);
    m_OutputCh_41.set(m_data_4);
    m_OutputCh_51.set(m_data_5);
    m_OutputCh_61.set(m_data_6);
    m_OutputCh_71.set(m_data_7);
    m_OutputCh_81.set(m_data_8);
    m_OutputCh_12.set(m_data_1);
    m_OutputCh_22.set(m_data_2);
    m_OutputCh_32.set(m_data_3);
    m_OutputCh_42.set(m_data_4);
    m_OutputCh_52.set(m_data_5);
    m_OutputCh_62.set(m_data_6);
    m_OutputCh_72.set(m_data_7);
    m_OutputCh_82.set(m_data_8);
    m_OutputCh_13.set(m_data_1);
    m_OutputCh_23.set(m_data_2);
    m_OutputCh_33.set(m_data_3);
    m_OutputCh_43.set(m_data_4);
    m_OutputCh_53.set(m_data_5);
    m_OutputCh_63.set(m_data_6);
    m_OutputCh_73.set(m_data_7);
    m_OutputCh_83.set(m_data_8);
    m_OutputCh_14.set(m_data_1);
    m_OutputCh_24.set(m_data_2);
    m_OutputCh_34.set(m_data_3);
    m_OutputCh_44.set(m_data_4);
    m_OutputCh_54.set(m_data_5);
    m_OutputCh_64.set(m_data_6);
    m_OutputCh_74.set(m_data_7);
    m_OutputCh_84.set(m_data_8);

    return dwRes;
}

