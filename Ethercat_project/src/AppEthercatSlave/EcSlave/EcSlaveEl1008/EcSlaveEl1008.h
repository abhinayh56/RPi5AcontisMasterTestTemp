#ifndef EC_SLAVE_EL1008_H
#define EC_SLAVE_EL1008_H

#include "EcSlaveBase.h"

namespace EcSlaveEl1008Data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6000, 1, bool, 1> Channel_1;
		PdoVariable<0x6010, 1, bool, 1> Channel_2;
		PdoVariable<0x6020, 1, bool, 1> Channel_3;
		PdoVariable<0x6030, 1, bool, 1> Channel_4;
		PdoVariable<0x6040, 1, bool, 1> Channel_5;
		PdoVariable<0x6050, 1, bool, 1> Channel_6;
		PdoVariable<0x6060, 1, bool, 1> Channel_7;
		PdoVariable<0x6070, 1, bool, 1> Channel_8;
	};
	#pragma pack(pop)
}

class EcSlaveEl1008 : public EcSlaveBase
{
public:
	EcSlaveEl1008(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl1008();

	virtual EC_T_DWORD registerTxPdo() override;

	virtual EC_T_DWORD registerRxPdo() override;

	virtual EC_T_DWORD transferTxPdo() override;

	virtual EC_T_DWORD transferRxPdo() override;

	virtual EC_T_DWORD processTxPdo() override;

	virtual EC_T_DWORD processRxPdo() override;

	virtual EC_T_DWORD registerPublisher() override;

	virtual EC_T_DWORD registerSubscriber() override;

	virtual EC_T_DWORD publishData() override;

	virtual EC_T_DWORD subscribeData() override;

	virtual EC_T_DWORD mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

protected:
	EcSlaveEl1008Data::TxPdo m_txPdo;

private:
	Data_store_element<bool> m_InputCh_11;
	Data_store_element<bool> m_InputCh_21;
	Data_store_element<bool> m_InputCh_31;
	Data_store_element<bool> m_InputCh_41;
	Data_store_element<bool> m_InputCh_51;
	Data_store_element<bool> m_InputCh_61;
	Data_store_element<bool> m_InputCh_71;
	Data_store_element<bool> m_InputCh_81;
	Data_store_element<bool> m_InputCh_12;
	Data_store_element<bool> m_InputCh_22;
	Data_store_element<bool> m_InputCh_32;
	Data_store_element<bool> m_InputCh_42;
	Data_store_element<bool> m_InputCh_52;
	Data_store_element<bool> m_InputCh_62;
	Data_store_element<bool> m_InputCh_72;
	Data_store_element<bool> m_InputCh_82;
	Data_store_element<bool> m_InputCh_13;
	Data_store_element<bool> m_InputCh_23;
	Data_store_element<bool> m_InputCh_33;
	Data_store_element<bool> m_InputCh_43;
	Data_store_element<bool> m_InputCh_53;
	Data_store_element<bool> m_InputCh_63;
	Data_store_element<bool> m_InputCh_73;
	Data_store_element<bool> m_InputCh_83;
	Data_store_element<bool> m_InputCh_14;
	Data_store_element<bool> m_InputCh_24;
	Data_store_element<bool> m_InputCh_34;
	Data_store_element<bool> m_InputCh_44;
	Data_store_element<bool> m_InputCh_54;
	Data_store_element<bool> m_InputCh_64;
	Data_store_element<bool> m_InputCh_74;
	Data_store_element<bool> m_InputCh_84;
};

#endif // EC_SLAVE_EL1008_H
