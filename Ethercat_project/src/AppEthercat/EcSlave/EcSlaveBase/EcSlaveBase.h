#ifndef EC_SLAVE_BASE_H
#define EC_SLAVE_BASE_H

#include "EcMaster.h"
#include <vector>
#include <string>
#include "EcPdoVariable.h"
#include "EcInputPdoRegister.h"
#include "EcOutputPdoRegister.h"
#include "EcInputPdoTransfer.h"
#include "EcOutputPdoTransfer.h"
#include "EcAllPdoRegister.h"
#include "EcAllPdoTransfer.h"
#include "EcTaskEthercatBase.h"

class EcSlaveBase : public EcTaskEthercatBase
{
public:
	EcSlaveBase(uint16_t slaveAddr, const std::string &slaveName);

	virtual ~EcSlaveBase();

	virtual EC_T_DWORD checkSlave() override;

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

	virtual uint16_t getAddress() override;

	virtual std::string& getName() override;

protected:
	uint16_t m_slaveAddr;
	std::string m_slaveName;
	EC_T_CFG_SLAVE_INFO m_slaveInfo;

private:
//	std::vector<Ec_pdo_element> m_rxPdoElementVector;
//	std::vector<Ec_pdo_element> m_txPdoElementVector;
};

#endif // EC_SLAVE_BASE_H
