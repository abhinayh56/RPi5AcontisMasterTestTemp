#ifndef EC_TASK_ETHERCAT_SLAVE_H
#define EC_TASK_ETHERCAT_SLAVE_H

#include <vector>
#include "EcTaskEthercatSlaveBase.h"
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

class EcTaskEthercatSlave
{
public:
    EcTaskEthercatSlave();

    virtual ~EcTaskEthercatSlave();

    EC_T_DWORD addSlaves();

    EC_T_DWORD cleanupTask();

    EC_T_DWORD checkSlave();

    EC_T_DWORD registerPdo();
    
	EC_T_DWORD registerPublisher();

	EC_T_DWORD registerSubscriber();

    EC_T_DWORD transferTxPdo();

    EC_T_DWORD transferRxPdo();

    EC_T_DWORD processTxPdo();

    EC_T_DWORD processRxPdo();

    EC_T_DWORD publishData();

    EC_T_DWORD subscribeData();

    EC_T_DWORD mainProcess();

    void dispTxPdo();

    void dispRxPdo();

private:
    std::vector<EcTaskEthercatSlaveBase*> m_EcTaskEthercatSlaveBaseVector;
    int m_numSlaves = 0;

    EC_T_DWORD addSlave(EcTaskEthercatSlaveBase* pSlave);
};

#endif // EC_TASK_ETHERCAT_SLAVE_H
