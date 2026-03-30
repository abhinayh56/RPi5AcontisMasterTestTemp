#ifndef EC_SLAVE_SSC_IO_MODULE_H
#define EC_SLAVE_SSC_IO_MODULE_H

#include "EcSlaveBase.h"

namespace EcSlaveSscIoModuleData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t> footpedal_1;
		PdoVariable<0x0006,  2, uint16_t> footpedal_2;
		PdoVariable<0x0006,  3, uint16_t> QUALITY_LEFT;
		PdoVariable<0x0006,  4, uint16_t> QUALITY_RIGHT;
		PdoVariable<0x0006,  5, uint16_t> GRIP_VAL_RIGHT;
		PdoVariable<0x0006,  6, uint16_t> GRIP_VAL_LEFT;
		PdoVariable<0x0006,  7,   double> X_LEFT;
		PdoVariable<0x0006,  8,   double> Y_LEFT;
		PdoVariable<0x0006,  9,   double> Z_LEFT;
		PdoVariable<0x0006, 10,   double> Q1_LEFT;
		PdoVariable<0x0006, 11,   double> Q2_LEFT;
		PdoVariable<0x0006, 12,   double> Q3_LEFT;
		PdoVariable<0x0006, 13,   double> Q4_LEFT;
		PdoVariable<0x0006, 14,   double> X_RIGHT;
		PdoVariable<0x0006, 15,   double> Y_RIGHT;
		PdoVariable<0x0006, 16,   double> Z_RIGHT;
		PdoVariable<0x0006, 17,   double> Q1_RIGHT;
		PdoVariable<0x0006, 18,   double> Q2_RIGHT;
		PdoVariable<0x0006, 19,   double> Q3_RIGHT;
		PdoVariable<0x0006, 20,   double> Q4_RIGHT;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t> IPC_TX_1;
		PdoVariable<0x0005,  2, uint16_t> IPC_RX_1;
	};
	#pragma pack(pop)
}

class EcSlaveSscIoModule : public EcSlaveBase
{
public:
	EcSlaveSscIoModule(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveSscIoModule();

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
	EcSlaveSscIoModuleData::TxPdo m_txPdo;
	EcSlaveSscIoModuleData::RxPdo m_rxPdo;
};

#endif // EC_SLAVE_SSC_IO_MODULE_H
