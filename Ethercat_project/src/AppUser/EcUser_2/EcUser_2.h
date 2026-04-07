#ifndef USER_2_H
#define USER_2_H

#include "EcTaskUserBase.h"

class EcUser_2 : public EcTaskUserBase
{
public:
	EcUser_2(uint16_t ecTaskUserId, const std::string &ecTaskUserName);

	~EcUser_2();

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

	Data_store_element<bool> m_OutputCh_1;
	Data_store_element<bool> m_OutputCh_2;
	Data_store_element<bool> m_OutputCh_3;
	Data_store_element<bool> m_OutputCh_4;
	Data_store_element<bool> m_OutputCh_5;
	Data_store_element<bool> m_OutputCh_6;
	Data_store_element<bool> m_OutputCh_7;
	Data_store_element<bool> m_OutputCh_8;

	bool m_data_1 = false;
	bool m_data_2 = false;
	bool m_data_3 = false;
	bool m_data_4 = false;
	bool m_data_5 = false;
	bool m_data_6 = false;
	bool m_data_7 = false;
	bool m_data_8 = false;
};

#endif // USER_2_H
