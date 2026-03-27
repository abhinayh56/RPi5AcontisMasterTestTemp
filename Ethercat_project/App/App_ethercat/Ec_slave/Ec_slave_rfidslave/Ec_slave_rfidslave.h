#ifndef EC_SLAVE_RFIDSLAVE_H
#define EC_SLAVE_RFIDSLAVE_H

#include "Ec_slave_base.h"

namespace Ec_slave_rfidslave_data
{
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t> Device_ID;
		PdoVariable<0x0006,  2, uint16_t> Second;
		PdoVariable<0x0006,  3, uint16_t> Minute;
		PdoVariable<0x0006,  4, uint16_t> Hour;
		PdoVariable<0x0006,  5, uint16_t> Day;
		PdoVariable<0x0006,  6, uint16_t> Month;
		PdoVariable<0x0006,  7, uint16_t> Year;
		PdoVariable<0x0006,  8, uint16_t> Roll_Offset;
		PdoVariable<0x0006,  9, uint16_t> Pitch_Offset;
		PdoVariable<0x0006, 10, uint16_t> Yaw_Offset;
		PdoVariable<0x0006, 11, uint16_t> Grip_Offset;
		PdoVariable<0x0006, 12, uint16_t> No_of_Usages;
		PdoVariable<0x0006, 13, uint16_t> Max_Usages;
		PdoVariable<0x0006, 14, uint16_t> Digital_Inputs;
		PdoVariable<0x0006, 15, uint16_t> Grip_Counts;
		PdoVariable<0x0006, 16, uint16_t> System_Number;
		PdoVariable<0x0006, 17, uint16_t> Device_UID;
		PdoVariable<0x0006, 18, uint16_t> Spare_Bytes;
		PdoVariable<0x0006, 19, uint16_t> MFG_Day;
		PdoVariable<0x0006, 20, uint16_t> MFG_Month;
		PdoVariable<0x0006, 21, uint16_t> MFG_Year;
	};

	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t> Acknowledge;
		PdoVariable<0x0005,  2, uint16_t> Second;
		PdoVariable<0x0005,  3, uint16_t> Minute;
		PdoVariable<0x0005,  4, uint16_t> Hour;
		PdoVariable<0x0005,  5, uint16_t> Day;
		PdoVariable<0x0005,  6, uint16_t> Month;
		PdoVariable<0x0005,  7, uint16_t> Year;
		PdoVariable<0x0005,  8, uint16_t> No_of_Usages;
		PdoVariable<0x0005,  9, uint16_t> Digital_Outputs;
		PdoVariable<0x0005, 10, uint16_t> Grip_Counts;
		PdoVariable<0x0005, 11, uint16_t> System_Number;
		PdoVariable<0x0005, 12, uint16_t> Led_Red;
		PdoVariable<0x0005, 13, uint16_t> Led_Green;
		PdoVariable<0x0005, 14, uint16_t> LED_Blue;
		PdoVariable<0x0005, 15, uint16_t> Spare_Bytes;
		PdoVariable<0x0005, 16, uint16_t> Roll_Offset;
		PdoVariable<0x0005, 17, uint16_t> Pitch_Offset;
		PdoVariable<0x0005, 18, uint16_t> Yaw_Offset;
		PdoVariable<0x0005, 19, uint16_t> Grip_Offset;
	};
}

class Ec_slave_rfidslave : public Ec_slave_base
{
public:
	Ec_slave_rfidslave(uint16_t slaveAddr);

	~Ec_slave_rfidslave();

	virtual EC_T_DWORD registerTxPdo();

	virtual EC_T_DWORD registerRxPdo();

	virtual EC_T_DWORD transferTxPdo();

	virtual EC_T_DWORD transferRxPdo();

	virtual EC_T_DWORD processTxPdo();

	virtual EC_T_DWORD processRxPdo();

	virtual EC_T_DWORD publishData();

	virtual EC_T_DWORD subscribeData();

	virtual EC_T_DWORD mainProcess();

	virtual void dispTxPdo();

	virtual void dispRxPdo();

protected:
	Ec_slave_rfidslave_data::TxPdo m_TxPdo;
	Ec_slave_rfidslave_data::RxPdo m_RxPdo;
};

#endif // EC_SLAVE_RFIDSLAVE_H
