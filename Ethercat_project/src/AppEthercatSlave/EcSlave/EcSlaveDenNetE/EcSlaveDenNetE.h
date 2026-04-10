#ifndef EC_SLAVE_DEN_NET_E_H
#define EC_SLAVE_DEN_NET_E_H

#include "EcCia402.h"

namespace EcSlaveDenNetEData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6041, 0, uint16_t, 16> Status_Word;
		PdoVariable<0x6064, 0,  int32_t, 32> Actual_position;
		PdoVariable<0x606C, 0,  int32_t, 32> Actual_velocity;
		PdoVariable<0x6061, 0,   int8_t,  8> Operation_mode_display;
		PdoVariable<0x6077, 0,  int16_t, 16> Torque_actual_value;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x6040, 0, uint16_t, 16> Control_Word;
        PdoVariable<0x607A, 0,  int32_t, 32> Position_set_point;
        PdoVariable<0x60FF, 0,  int32_t, 32> Velocity_set_point;
        PdoVariable<0x6060, 0,   int8_t,  8> Operation_mode;
        PdoVariable<0x6071, 0,  int16_t, 16> Target_torque;
	};
	#pragma pack(pop)
}

class EcSlaveDenNetE : public EcCia402
{
public:
	EcSlaveDenNetE(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveDenNetE();

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
	EcSlaveDenNetEData::TxPdo m_txPdo;
	EcSlaveDenNetEData::RxPdo m_rxPdo;

	Data_store_element<bool> m_InputCh_1;
	Data_store_element<bool> m_InputCh_2;
	Data_store_element<bool> m_InputCh_3;
	Data_store_element<bool> m_InputCh_4;
	Data_store_element<bool> m_InputCh_5;
	Data_store_element<bool> m_InputCh_6;
	Data_store_element<bool> m_InputCh_7;
	Data_store_element<bool> m_InputCh_8;

	bool m_data_1 = false;
	bool m_data_2 = false;
	bool m_data_3 = false;
	bool m_data_4 = false;
	bool m_data_5 = false;
	bool m_data_6 = false;
	bool m_data_7 = false;
	bool m_data_8 = false;

private:
	double angle_command = 0;
};

#endif // EC_SLAVE_DEN_NET_E_H
