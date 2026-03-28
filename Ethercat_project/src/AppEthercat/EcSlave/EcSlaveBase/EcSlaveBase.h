#ifndef EC_SLAVE_BASE_H
#define EC_SLAVE_BASE_H

#include "EcMaster.h"
#include <vector>
#include <string>
#include "EcPdoElement.h"

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

protected:
	uint16_t m_slaveAddr;
	std::string m_slaveName;

private:
//	std::vector<Ec_pdo_element> m_rxPdoElementVector;
//	std::vector<Ec_pdo_element> m_txPdoElementVector;
};

#endif // EC_SLAVE_BASE_H
