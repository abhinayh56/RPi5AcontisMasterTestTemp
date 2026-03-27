#ifndef EC_SLAVE_PITCH_DRIVE_H
#define EC_SLAVE_PITCH_DRIVE_H

#include "Ec_slave_base.h"

namespace Ec_slave_pitch_drive_data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6064, 0,  int32_t> ACT_POS;
		PdoVariable<0x6041, 0, uint16_t> STATUS_WD;
		PdoVariable<0x6077, 0,  int16_t> ACT_TOR;
		PdoVariable<0x6061, 0,  uint8_t> OPMODE_DISP;
		PdoVariable<0x603F, 0, uint16_t> ERROR_CODE;
		PdoVariable<0x3002, 0,  uint8_t> DIG_IN;
		PdoVariable<0x606C, 0,  int32_t> ACT_VEL;
		PdoVariable<0x3007, 0, uint16_t> ADC_VAL;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
		PdoVariable<0x607A, 0,  int32_t> TARGET_POSE;
		PdoVariable<0x6040, 0, uint16_t> CONTROL_WD;
		PdoVariable<0x6071, 0,  int16_t> TARGET_TORQ;
		PdoVariable<0x6060, 0,  uint8_t> OP_MODE;
		PdoVariable<0x3004, 0, uint16_t> ERROR_CLEAR;
		PdoVariable<0x3001, 0,  uint8_t> DIG_OUT;
		PdoVariable<0x60FF, 0,  int32_t> TARGET_VEL;
	};
	#pragma pack(pop)
}

class Ec_slave_pitch_drive : public Ec_slave_base
{
public:
	Ec_slave_pitch_drive(uint16_t slaveAddr, const std::string &slaveName);

	~Ec_slave_pitch_drive();

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
	Ec_slave_pitch_drive_data::TxPdo m_TxPdo;
	Ec_slave_pitch_drive_data::RxPdo m_RxPdo;
};

#endif // EC_SLAVE_PITCH_DRIVE_H
