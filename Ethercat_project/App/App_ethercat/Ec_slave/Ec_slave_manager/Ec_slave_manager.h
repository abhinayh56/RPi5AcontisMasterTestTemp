#ifndef EC_SLAVE_MANAGER_H
#define EC_SLAVE_MANAGER_H

#include <vector>
#include "EcType.h"
#include "Ec_slave_base.h"

class Ec_slave_manager
{
public:
    Ec_slave_manager();

    ~Ec_slave_manager();

    EC_T_DWORD addSlave(Ec_slave_base* pSlave);

    EC_T_DWORD cleanup();

    EC_T_DWORD registerPdo();

    EC_T_DWORD cyclicProcess();

private:
    EC_T_DWORD transferTxPdo();

    EC_T_DWORD transferRxPdo();

    EC_T_DWORD processTxPdo();

    EC_T_DWORD processRxPdo();

    EC_T_DWORD publishData();

    EC_T_DWORD subscribeData();

    EC_T_DWORD mainProcess();

    void dispTxPdo();

    void dispRxPdo();

    std::vector<Ec_slave_base*> m_slaveVector;
    int m_numSlaves = 0;
};

#endif // EC_SLAVE_MANAGER_H