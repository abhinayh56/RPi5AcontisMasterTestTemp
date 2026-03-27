#ifndef EC_SLAVE_MANAGER_H
#define EC_SLAVE_MANAGER_H

#include <vector>
#include "EcType.h"
#include "Ec_slave_base.h"
#include "Ec_slave_el1008.h"
#include "Ec_slave_el2008.h"
#include "Ec_slave_el3024.h"
#include "Ec_slave_el6002.h"
#include "Ec_slave_io_module.h"
#include "Ec_slave_encbrkctrl.h"
#include "Ec_slave_rfidslave.h"
#include "Ec_slave_den_net_e.h"
#include "Ec_slave_elmo_drive.h"
#include "Ec_slave_ipos2401_mx_cat.h"
#include "Ec_slave_pitch_drive.h"

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
    std::vector<Ec_slave_base*> m_slaveVector;
    int m_numSlaves = 0;

    EC_T_DWORD transferTxPdo();

    EC_T_DWORD transferRxPdo();

    EC_T_DWORD processTxPdo();

    EC_T_DWORD processRxPdo();

    EC_T_DWORD publishData();

    EC_T_DWORD subscribeData();

    EC_T_DWORD mainProcess();

    void dispTxPdo();

    void dispRxPdo();
};

#endif // EC_SLAVE_MANAGER_H
