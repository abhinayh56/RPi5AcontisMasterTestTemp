#ifndef EC_SLAVE_BASE_H
#define EC_SLAVE_BASE_H

#include "EcMaster.h"
#include <vector>
#include "Ec_pdo_element.h"

class Ec_slave_base
{
public:
	Ec_slave_base(uint16_t slaveAddr);

	virtual ~Ec_slave_base();

	virtual EC_T_DWORD registerTxPdos();

	virtual EC_T_DWORD registerRxPdos();

	virtual EC_T_DWORD transferTxPdo();

	virtual EC_T_DWORD transferRxPdo();

	virtual EC_T_DWORD processTxPdo();

	virtual EC_T_DWORD processRxPdo();

	virtual EC_T_DWORD publishData();

	virtual EC_T_DWORD subscribeData();

	virtual EC_T_DWORD mainProcess();

protected:
	uint16_t m_SlaveAddr = 0;

private:
//	std::vector<Ec_pdo_element> m_rxPdoElementVector;
//	std::vector<Ec_pdo_element> m_txPdoElementVector;
};

#endif // EC_SLAVE_BASE_H
