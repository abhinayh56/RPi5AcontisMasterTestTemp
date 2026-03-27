#ifndef EC_SLAVE_ELMO_DRIVE_H
#define EC_SLAVE_ELMO_DRIVE_H

#include "Ec_slave_base.h"

namespace Ec_slave_elmo_drive_data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6064, 0,  int32_t> Position_actual_valuie;
		PdoVariable<0x60FD, 0,  int32_t> Digital_Inputs;
		PdoVariable<0x6041, 0, uint16_t> Status_word;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x607A, 0,  int32_t> Target_Position;
		PdoVariable<0x60FE, 0, uint32_t> Digital_Outputs;
		PdoVariable<0x6040, 0, uint16_t> Control_word;
	};
	#pragma pack(pop)
}

class Ec_slave_elmo_drive : public Ec_slave_base
{
public:
	Ec_slave_elmo_drive(uint16_t slaveAddr);

	~Ec_slave_elmo_drive();

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
	Ec_slave_elmo_drive_data::TxPdo m_TxPdo;
	Ec_slave_elmo_drive_data::RxPdo m_RxPdo;
};

#endif // EC_SLAVE_ELMO_DRIVE_H
