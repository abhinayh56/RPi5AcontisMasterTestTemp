#ifndef EC_SLAVE_DEN_NET_E_H
#define EC_SLAVE_DEN_NET_E_H

#include "Ec_slave_base.h"

namespace Ec_slave_den_net_e
{
	struct TxPdo
	{
		PdoVariable<0x6041, 0, uint16_t> Status_Word;
		PdoVariable<0x6064, 0,  int32_t> Actual_position;
		PdoVariable<0x606C, 0,  int32_t> Actual_velocity;
		PdoVariable<0x6061, 0,   int8_t> Operation_mode_display;
	};

	struct RxPdo
	{
        PdoVariable<0x6040, 0, uint16_t> Control_Word;
        PdoVariable<0x607A, 0,  int32_t> Position_set_point;
        PdoVariable<0x60FF, 0,  int32_t> Velocity_set_point;
        PdoVariable<0x6060, 0,   int8_t> Operation_mode;
	};
}

class Ec_slave_den_net_e : public Ec_slave_base
{
public:
	Ec_slave_den_net_e(uint16_t slaveAddr);

	~Ec_slave_den_net_e();

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
	Ec_slave_den_net_e::TxPdo m_TxPdo;
	Ec_slave_den_net_e::RxPdo m_RxPdo;
};

#endif // EC_SLAVE_DEN_NET_E_H
