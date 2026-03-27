#ifndef PITCH_DRIVE_H
#define PITCH_DRIVE_H

#include "Ec_slave_base.h"

namespace Ec_slave_pitch_drive_data
{
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
}

class Ec_slave_pitch_drive : public Ec_slave_base
{
public:
	Ec_slave_pitch_drive(uint16_t slaveAddr);

	~Ec_slave_pitch_drive();

	virtual EC_T_DWORD registerTxPdos();

	virtual EC_T_DWORD registerRxPdos();

	virtual EC_T_DWORD transferTxPdo();

	virtual EC_T_DWORD processTxPdo();

	virtual EC_T_DWORD publishData();

	virtual EC_T_DWORD subscribeData();

	virtual EC_T_DWORD mainProcess();

	virtual EC_T_DWORD processRxPdo();

	virtual EC_T_DWORD transferRxPdo();

protected:
	Ec_slave_pitch_drive_data::TxPdo m_TxPdo;
};

#endif // PITCH_DRIVE_H
