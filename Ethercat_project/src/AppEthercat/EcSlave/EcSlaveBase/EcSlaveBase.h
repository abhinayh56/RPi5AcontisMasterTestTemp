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
#include "EcSlavePdo.h"

class EcSlaveBase
{
public:
	EcSlaveBase(uint16_t slaveAddr, const std::string &slaveName);

	virtual ~EcSlaveBase();

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

	virtual uint16_t getAddress();

	virtual std::string& getName();

protected:
	uint16_t m_slaveAddr;
	std::string m_slaveName;
	EC_T_CFG_SLAVE_INFO m_slaveInfo;

private:
//	std::vector<Ec_pdo_element> m_rxPdoElementVector;
//	std::vector<Ec_pdo_element> m_txPdoElementVector;
};

#endif // EC_SLAVE_BASE_H
