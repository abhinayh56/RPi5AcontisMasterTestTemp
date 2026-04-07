#ifndef USER_1_H
#define USER_1_H

#include "EcTaskUserBase.h"

class EcUser_1 : public EcTaskUserBase
{
public:
	EcUser_1(uint16_t ecTaskUserId, const std::string &ecTaskUserName);

	~EcUser_1();

	EC_T_DWORD registerPublisher() override;

	EC_T_DWORD registerSubscriber() override;

	EC_T_DWORD publishData() override;

	EC_T_DWORD subscribeData() override;

	EC_T_DWORD mainProcess() override;
        
private:
	Data_store_element<bool> m_InputCh_1;
	Data_store_element<bool> m_InputCh_2;
	Data_store_element<bool> m_InputCh_3;
	Data_store_element<bool> m_InputCh_4;
	Data_store_element<bool> m_InputCh_5;
	Data_store_element<bool> m_InputCh_6;
	Data_store_element<bool> m_InputCh_7;
	Data_store_element<bool> m_InputCh_8;

	Data_store_element<bool> m_OutputCh_11;
	Data_store_element<bool> m_OutputCh_21;
	Data_store_element<bool> m_OutputCh_31;
	Data_store_element<bool> m_OutputCh_41;
	Data_store_element<bool> m_OutputCh_51;
	Data_store_element<bool> m_OutputCh_61;
	Data_store_element<bool> m_OutputCh_71;
	Data_store_element<bool> m_OutputCh_81;

	Data_store_element<bool> m_OutputCh_12;
	Data_store_element<bool> m_OutputCh_22;
	Data_store_element<bool> m_OutputCh_32;
	Data_store_element<bool> m_OutputCh_42;
	Data_store_element<bool> m_OutputCh_52;
	Data_store_element<bool> m_OutputCh_62;
	Data_store_element<bool> m_OutputCh_72;
	Data_store_element<bool> m_OutputCh_82;

	Data_store_element<bool> m_OutputCh_13;
	Data_store_element<bool> m_OutputCh_23;
	Data_store_element<bool> m_OutputCh_33;
	Data_store_element<bool> m_OutputCh_43;
	Data_store_element<bool> m_OutputCh_53;
	Data_store_element<bool> m_OutputCh_63;
	Data_store_element<bool> m_OutputCh_73;
	Data_store_element<bool> m_OutputCh_83;

	Data_store_element<bool> m_OutputCh_14;
	Data_store_element<bool> m_OutputCh_24;
	Data_store_element<bool> m_OutputCh_34;
	Data_store_element<bool> m_OutputCh_44;
	Data_store_element<bool> m_OutputCh_54;
	Data_store_element<bool> m_OutputCh_64;
	Data_store_element<bool> m_OutputCh_74;
	Data_store_element<bool> m_OutputCh_84;

	bool m_data_1 = false;
	bool m_data_2 = false;
	bool m_data_3 = false;
	bool m_data_4 = false;
	bool m_data_5 = false;
	bool m_data_6 = false;
	bool m_data_7 = false;
	bool m_data_8 = false;
};

#endif // USER_1_H
