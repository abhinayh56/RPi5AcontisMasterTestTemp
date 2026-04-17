#ifndef EC_SLAVE_EL3024_H
#define EC_SLAVE_EL3024_H

#include "EcSlaveBase.h"

namespace EcSlaveEl3024Data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6000, 17, int16_t, 16> Al_Compact_Channel_1_Value;
		PdoVariable<0x6010, 17, int16_t, 16> Al_Compact_Channel_2_Value;
	};
	#pragma pack(pop)
}

class EcSlaveEl3024 : public EcSlaveBase
{
public:
	EcSlaveEl3024(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl3024();

	virtual EC_T_DWORD registerTxPdo() override;

	virtual EC_T_DWORD registerRxPdo() override;

	virtual EC_T_DWORD transferTxPdo() override;

	virtual EC_T_DWORD transferRxPdo() override;

	virtual EC_T_DWORD processTxPdo() override;

	virtual EC_T_DWORD processRxPdo() override;

	virtual EC_T_DWORD registerPublisher() override;

	virtual EC_T_DWORD registerSubscriber() override;

	virtual EC_T_DWORD publishData() override;

	virtual EC_T_DWORD subscribeData() override;

	virtual EC_T_DWORD mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

protected:
	EcSlaveEl3024Data::TxPdo m_txPdo;
};

#endif // EC_SLAVE_EL3024_H
