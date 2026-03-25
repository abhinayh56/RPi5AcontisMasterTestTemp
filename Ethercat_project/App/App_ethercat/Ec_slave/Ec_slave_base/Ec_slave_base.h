#ifndef EC_SLAVE_BASE_H
#define EC_SLAVE_BASE_H

#include "EcMaster.h"

class Ec_slave_base
{
public:
	Ec_slave_base();

	~Ec_slave_base();

	EC_T_DWORD transferTxPdo();

	EC_T_DWORD processTxPdo();

	EC_T_DWORD runMainProcess();

	EC_T_DWORD processRxPdo();

	EC_T_DWORD transferRxPdo();

private:
};

#endif // EC_SLAVE_BASE_H
