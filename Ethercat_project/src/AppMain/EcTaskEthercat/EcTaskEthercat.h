#ifndef EC_TASK_ETHERCAT_H
#define EC_TASK_ETHERCAT_H

#include <vector>
#include "EcTaskEthercatBase.h"
#include "EcSlaveEl1008.h"
#include "EcSlaveEl2008.h"
#include "EcSlaveEl3024.h"
#include "EcSlaveEl6002.h"
#include "EcSlaveSscIoModule.h"
#include "EcSlaveEncbrkctrl.h"
#include "EcSlaveRfidslave.h"
#include "EcSlaveDenNetE.h"
#include "EcSlaveElmoDrive.h"
#include "EcSlaveIpos2401MxCat.h"
#include "EcSlavePitchDrive.h"

class EcTaskEthercat
{
public:
    EcTaskEthercat();

    virtual ~EcTaskEthercat();

    EC_T_DWORD addSlave(EcTaskEthercatBase* pSlave);

    EC_T_DWORD cleanupTask();

    EC_T_DWORD configTask();

    EC_T_DWORD cyclicTask();

private:
    std::vector<EcTaskEthercatBase*> m_ecTaskEthercatBaseVector;
    int m_numSlaves = 0;

    EC_T_DWORD checkSlave();

    EC_T_DWORD registerPdo();
    
    EC_T_DWORD transferTxPdo();

    EC_T_DWORD transferRxPdo();

    EC_T_DWORD processTxPdo();

    EC_T_DWORD processRxPdo();

    EC_T_DWORD registerPublisher();

    EC_T_DWORD registerSubscriber();

    EC_T_DWORD publishData();

    EC_T_DWORD subscribeData();

    EC_T_DWORD mainProcess();

    void dispTxPdo();

    void dispRxPdo();
};

#endif // EC_TASK_ETHERCAT_H
