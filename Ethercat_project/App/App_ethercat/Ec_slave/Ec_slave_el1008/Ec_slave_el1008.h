#ifndef EC_SLAVE_EL1008_H
#define EC_SLAVE_EL1008_H

#include "Ec_slave_base.h"

namespace Ec_slave_el1008_data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6000, 1, bool> Channel_1;
		PdoVariable<0x6010, 1, bool> Channel_2;
		PdoVariable<0x6020, 1, bool> Channel_3;
		PdoVariable<0x6030, 1, bool> Channel_4;
		PdoVariable<0x6040, 1, bool> Channel_5;
		PdoVariable<0x6050, 1, bool> Channel_6;
		PdoVariable<0x6060, 1, bool> Channel_7;
		PdoVariable<0x6070, 1, bool> Channel_8;
	};
	#pragma pack(pop)
}

class Ec_slave_el1008 : public Ec_slave_base
{
public:
	Ec_slave_el1008(uint16_t slaveAddr);

	~Ec_slave_el1008();

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
	Ec_slave_el1008_data::TxPdo m_TxPdo;
};

#endif // EC_SLAVE_EL1008_H
