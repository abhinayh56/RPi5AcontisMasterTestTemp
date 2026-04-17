#ifndef EC_SLAVE_SSC_IO_MODULE_H
#define EC_SLAVE_SSC_IO_MODULE_H

#include "EcSlaveBase.h"

namespace EcSlaveSscIoModuleData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t, 16> footpedal_1;
		PdoVariable<0x0006,  2, uint16_t, 16> footpedal_2;
		PdoVariable<0x0006,  3, uint16_t, 16> QUALITY_LEFT;
		PdoVariable<0x0006,  4, uint16_t, 16> QUALITY_RIGHT;
		PdoVariable<0x0006,  5, uint16_t, 16> GRIP_VAL_RIGHT;
		PdoVariable<0x0006,  6, uint16_t, 16> GRIP_VAL_LEFT;
		PdoVariable<0x0006,  7,   double, 64> X_LEFT;
		PdoVariable<0x0006,  8,   double, 64> Y_LEFT;
		PdoVariable<0x0006,  9,   double, 64> Z_LEFT;
		PdoVariable<0x0006, 10,   double, 64> Q1_LEFT;
		PdoVariable<0x0006, 11,   double, 64> Q2_LEFT;
		PdoVariable<0x0006, 12,   double, 64> Q3_LEFT;
		PdoVariable<0x0006, 13,   double, 64> Q4_LEFT;
		PdoVariable<0x0006, 14,   double, 64> X_RIGHT;
		PdoVariable<0x0006, 15,   double, 64> Y_RIGHT;
		PdoVariable<0x0006, 16,   double, 64> Z_RIGHT;
		PdoVariable<0x0006, 17,   double, 64> Q1_RIGHT;
		PdoVariable<0x0006, 18,   double, 64> Q2_RIGHT;
		PdoVariable<0x0006, 19,   double, 64> Q3_RIGHT;
		PdoVariable<0x0006, 20,   double, 64> Q4_RIGHT;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t, 16> IPC_TX_1;
		PdoVariable<0x0005,  2, uint16_t, 16> IPC_RX_1;
	};
	#pragma pack(pop)
}

class EcSlaveSscIoModule : public EcSlaveBase
{
public:
	EcSlaveSscIoModule(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveSscIoModule();

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
	EcSlaveSscIoModuleData::TxPdo m_txPdo;
	EcSlaveSscIoModuleData::RxPdo m_rxPdo;
};

#endif // EC_SLAVE_SSC_IO_MODULE_H
