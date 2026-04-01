#ifndef EC_SLAVE_ELMO_DRIVE_H
#define EC_SLAVE_ELMO_DRIVE_H

#include "EcSlaveBase.h"

namespace EcSlaveElmoDriveData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6064, 0,  int32_t, 32> Position_actual_valuie;
		PdoVariable<0x60FD, 0,  int32_t, 32> Digital_Inputs;
		PdoVariable<0x6041, 0, uint16_t, 16> Status_word;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x607A, 0,  int32_t, 32> Target_Position;
		PdoVariable<0x60FE, 0, uint32_t, 32> Digital_Outputs;
		PdoVariable<0x6040, 0, uint16_t, 16> Control_word;
	};
	#pragma pack(pop)
}

class EcSlaveElmoDrive : public EcSlaveBase
{
public:
	EcSlaveElmoDrive(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveElmoDrive();

	virtual EC_T_DWORD registerTxPdo() override;

	virtual EC_T_DWORD registerRxPdo() override;

	virtual EC_T_DWORD transferTxPdo() override;

	virtual EC_T_DWORD transferRxPdo() override;

	virtual EC_T_DWORD processTxPdo() override;

	virtual EC_T_DWORD processRxPdo() override;

	virtual EC_T_DWORD publishData() override;

	virtual EC_T_DWORD subscribeData() override;

	virtual EC_T_DWORD mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

protected:
	EcSlaveElmoDriveData::TxPdo m_txPdo;
	EcSlaveElmoDriveData::RxPdo m_rxPdo;
};

#endif // EC_SLAVE_ELMO_DRIVE_H
