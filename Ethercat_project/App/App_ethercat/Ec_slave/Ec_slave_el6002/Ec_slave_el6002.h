#ifndef EC_SLAVE_EL6002_H
#define EC_SLAVE_EL6002_H

#include "Ec_slave_base.h"

namespace Ec_slave_el6002_data
{
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t> Status;
		PdoVariable<0x0006,  1,  uint8_t> Data_In_0;
		PdoVariable<0x0006,  2,  uint8_t> Second;
		PdoVariable<0x0006,  3,  uint8_t> Minute;
		PdoVariable<0x0006,  4,  uint8_t> Hour;
		PdoVariable<0x0006,  5,  uint8_t> Day;
		PdoVariable<0x0006,  6,  uint8_t> Month;
		PdoVariable<0x0006,  7,  uint8_t> Year;
		PdoVariable<0x0006,  8,  uint8_t> Roll_Offset;
		PdoVariable<0x0006,  9,  uint8_t> Pitch_Offset;
		PdoVariable<0x0006, 10,  uint8_t> Yaw_Offset;
		PdoVariable<0x0006, 11,  uint8_t> Grip_Offset;
		PdoVariable<0x0006, 12,  uint8_t> No_of_Usages;
		PdoVariable<0x0006, 13,  uint8_t> Max_Usages;
		PdoVariable<0x0006, 14,  uint8_t> Digital_Inputs;
		PdoVariable<0x0006, 15,  uint8_t> Grip_Counts;
		PdoVariable<0x0006, 16,  uint8_t> System_Number;
		PdoVariable<0x0006, 17,  uint8_t> Device_UID;
		PdoVariable<0x0006, 18,  uint8_t> Spare_Bytes;
		PdoVariable<0x0006, 19,  uint8_t> MFG_Day;
		PdoVariable<0x0006, 20,  uint8_t> MFG_Month;
		PdoVariable<0x0006, 21,  uint8_t> MFG_Year;
		PdoVariable<0x0006, 22,  uint16_t> MFG_Year;
	};

	struct RxPdo
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
		PdoVariable<0x0006, 22, uint16_t> MFG_Year;
	};
}

class Ec_slave_el6002 : public Ec_slave_base
{
public:
	Ec_slave_el6002(uint16_t slaveAddr);

	~Ec_slave_el6002();

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
	Ec_slave_el6002_data::TxPdo m_TxPdo;
	Ec_slave_el6002_data::RxPdo m_RxPdo;
};

#endif // EC_SLAVE_EL6002_H
