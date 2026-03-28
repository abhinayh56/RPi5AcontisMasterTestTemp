#ifndef EC_SLAVE_EL2008_H
#define EC_SLAVE_EL2008_H

#include "EcSlaveBase.h"

namespace EcSlaveEl2008Data
{
	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x7000, 1, bool> Channel_1;
        PdoVariable<0x7010, 1, bool> Channel_2;
        PdoVariable<0x7020, 1, bool> Channel_3;
        PdoVariable<0x7030, 1, bool> Channel_4;
        PdoVariable<0x7040, 1, bool> Channel_5;
        PdoVariable<0x7050, 1, bool> Channel_6;
        PdoVariable<0x7060, 1, bool> Channel_7;
        PdoVariable<0x7070, 1, bool> Channel_8;
	};
	#pragma pack(pop)
}

class EcSlaveEl2008 : public EcSlaveBase
{
public:
	EcSlaveEl2008(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl2008();

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
	EcSlaveEl2008Data::RxPdo m_rxPdo;
};

#endif // EC_SLAVE_EL2008_H
