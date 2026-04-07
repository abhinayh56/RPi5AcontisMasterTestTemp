#ifndef EC_SLAVE_PITCH_DRIVE_H
#define EC_SLAVE_PITCH_DRIVE_H

#include "EcSlaveBase.h"

namespace EcSlavePitchDriveData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6064, 0,  int32_t, 32> ACT_POS;
		PdoVariable<0x6041, 0, uint16_t, 16> STATUS_WD;
		PdoVariable<0x6077, 0,  int16_t, 16> ACT_TOR;
		PdoVariable<0x6061, 0,  uint8_t,  8> OPMODE_DISP;
		PdoVariable<0x603F, 0, uint16_t, 16> ERROR_CODE;
		PdoVariable<0x3002, 0,  uint8_t,  8> DIG_IN;
		PdoVariable<0x606C, 0,  int32_t, 32> ACT_VEL;
		PdoVariable<0x3007, 0, uint16_t, 16> ADC_VAL;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
		PdoVariable<0x607A, 0,  int32_t, 32> TARGET_POSE;
		PdoVariable<0x6040, 0, uint16_t, 16> CONTROL_WD;
		PdoVariable<0x6071, 0,  int16_t, 16> TARGET_TORQ;
		PdoVariable<0x6060, 0,  uint8_t,  8> OP_MODE;
		PdoVariable<0x3004, 0, uint16_t, 16> ERROR_CLEAR;
		PdoVariable<0x3001, 0,  uint8_t,  8> DIG_OUT;
		PdoVariable<0x60FF, 0,  int32_t, 32> TARGET_VEL;
	};
	#pragma pack(pop)
}

class EcSlavePitchDrive : public EcSlaveBase
{
public:
	EcSlavePitchDrive(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlavePitchDrive();

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
	EcSlavePitchDriveData::TxPdo m_txPdo;
	EcSlavePitchDriveData::RxPdo m_rxPdo;

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
};

#endif // EC_SLAVE_PITCH_DRIVE_H
