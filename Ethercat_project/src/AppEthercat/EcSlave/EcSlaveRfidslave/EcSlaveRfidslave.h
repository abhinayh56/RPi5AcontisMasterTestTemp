#ifndef EC_SLAVE_RFIDSLAVE_H
#define EC_SLAVE_RFIDSLAVE_H

#include "EcSlaveBase.h"

namespace EcSlaveRfidslaveData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t, 16> Device_ID;
		PdoVariable<0x0006,  2, uint16_t, 16> Second;
		PdoVariable<0x0006,  3, uint16_t, 16> Minute;
		PdoVariable<0x0006,  4, uint16_t, 16> Hour;
		PdoVariable<0x0006,  5, uint16_t, 16> Day;
		PdoVariable<0x0006,  6, uint16_t, 16> Month;
		PdoVariable<0x0006,  7, uint16_t, 16> Year;
		PdoVariable<0x0006,  8, uint16_t, 16> Roll_Offset;
		PdoVariable<0x0006,  9, uint16_t, 16> Pitch_Offset;
		PdoVariable<0x0006, 10, uint16_t, 16> Yaw_Offset;
		PdoVariable<0x0006, 11, uint16_t, 16> Grip_Offset;
		PdoVariable<0x0006, 12, uint16_t, 16> No_of_Usages;
		PdoVariable<0x0006, 13, uint16_t, 16> Max_Usages;
		PdoVariable<0x0006, 14, uint16_t, 16> Digital_Inputs;
		PdoVariable<0x0006, 15, uint16_t, 16> Grip_Counts;
		PdoVariable<0x0006, 16, uint16_t, 16> System_Number;
		PdoVariable<0x0006, 17, uint16_t, 16> Device_UID;
		PdoVariable<0x0006, 18, uint16_t, 16> Spare_Bytes;
		PdoVariable<0x0006, 19, uint16_t, 16> MFG_Day;
		PdoVariable<0x0006, 20, uint16_t, 16> MFG_Month;
		PdoVariable<0x0006, 21, uint16_t, 16> MFG_Year;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t, 16> Acknowledge;
		PdoVariable<0x0005,  2, uint16_t, 16> Second;
		PdoVariable<0x0005,  3, uint16_t, 16> Minute;
		PdoVariable<0x0005,  4, uint16_t, 16> Hour;
		PdoVariable<0x0005,  5, uint16_t, 16> Day;
		PdoVariable<0x0005,  6, uint16_t, 16> Month;
		PdoVariable<0x0005,  7, uint16_t, 16> Year;
		PdoVariable<0x0005,  8, uint16_t, 16> No_of_Usages;
		PdoVariable<0x0005,  9, uint16_t, 16> Digital_Outputs;
		PdoVariable<0x0005, 10, uint16_t, 16> Grip_Counts;
		PdoVariable<0x0005, 11, uint16_t, 16> System_Number;
		PdoVariable<0x0005, 12, uint16_t, 16> Led_Red;
		PdoVariable<0x0005, 13, uint16_t, 16> Led_Green;
		PdoVariable<0x0005, 14, uint16_t, 16> LED_Blue;
		PdoVariable<0x0005, 15, uint16_t, 16> Spare_Bytes;
		PdoVariable<0x0005, 16, uint16_t, 16> Roll_Offset;
		PdoVariable<0x0005, 17, uint16_t, 16> Pitch_Offset;
		PdoVariable<0x0005, 18, uint16_t, 16> Yaw_Offset;
		PdoVariable<0x0005, 19, uint16_t, 16> Grip_Offset;
	};
	#pragma pack(pop)
}

class EcSlaveRfidslave : public EcSlaveBase
{
public:
	EcSlaveRfidslave(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveRfidslave();

	virtual EC_T_DWORD registerTxPdo() override;

	virtual EC_T_DWORD registerRxPdo() override;

	virtual EC_T_DWORD transferTxPdo() override;

	virtual EC_T_DWORD transferRxPdo() override;

	virtual EC_T_DWORD processTxPdo() override;

	virtual EC_T_DWORD processRxPdo() override;

	virtual EC_T_DWORD publishData() override;

	virtual EC_T_DWORD subscribeData() override;

	virtual EC_T_DWORD mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

protected:
	EcSlaveRfidslaveData::TxPdo m_txPdo;
	EcSlaveRfidslaveData::RxPdo m_rxPdo;
};

#endif // EC_SLAVE_RFIDSLAVE_H
