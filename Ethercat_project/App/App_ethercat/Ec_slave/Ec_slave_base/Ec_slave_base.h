#ifndef EC_SLAVE_BASE_H
#define EC_SLAVE_BASE_H

#include "EcMaster.h"

class Ec_slave_base
{
public:
	Ec_slave_base();

	virtual ~Ec_slave_base();

	virtual EC_T_DWORD transferTxPdo();

	virtual EC_T_DWORD processTxPdo();

	virtual EC_T_DWORD publishData();

	virtual EC_T_DWORD subscribeData();

	virtual EC_T_DWORD mainProcess();

	virtual EC_T_DWORD processRxPdo();

	virtual EC_T_DWORD transferRxPdo();

private:
};

#endif // EC_SLAVE_BASE_H
