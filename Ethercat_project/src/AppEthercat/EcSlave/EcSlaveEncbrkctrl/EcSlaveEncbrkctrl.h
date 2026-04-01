#ifndef EC_SLAVE_ENCBRKCTRL_H
#define EC_SLAVE_ENCBRKCTRL_H

#include "EcSlaveBase.h"

namespace EcSlaveEncbrkctrlData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint32_t, 32> ENC1;
		PdoVariable<0x0006,  2, uint32_t, 32> ENC2;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t, 16> BRK1;
		PdoVariable<0x0005,  2, uint16_t, 16> BRK2;
	};
	#pragma pack(pop)
}

class EcSlaveEncbrkctrl : public EcSlaveBase
{
public:
	EcSlaveEncbrkctrl(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEncbrkctrl();

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
	EcSlaveEncbrkctrlData::TxPdo m_txPdo;
	EcSlaveEncbrkctrlData::RxPdo m_rxPdo;
};

#endif // EC_SLAVE_ENCBRKCTRL_H
