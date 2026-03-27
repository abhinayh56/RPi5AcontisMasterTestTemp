#ifndef EC_SLAVE_ENCBRKCTRL_H
#define EC_SLAVE_ENCBRKCTRL_H

#include "Ec_slave_base.h"

namespace Ec_slave_encbrkctrl_data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint32_t> ENC1;
		PdoVariable<0x0006,  2, uint32_t> ENC2;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t> BRK1;
		PdoVariable<0x0005,  2, uint16_t> BRK2;
	};
	#pragma pack(pop)
}

class Ec_slave_encbrkctrl : public Ec_slave_base
{
public:
	Ec_slave_encbrkctrl(uint16_t slaveAddr);

	~Ec_slave_encbrkctrl();

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
	Ec_slave_encbrkctrl_data::TxPdo m_TxPdo;
	Ec_slave_encbrkctrl_data::RxPdo m_RxPdo;
};

#endif // EC_SLAVE_ENCBRKCTRL_H
