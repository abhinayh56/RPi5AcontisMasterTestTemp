#ifndef EC_TASK_ETHERCAT_SLAVE_SERVO_H
#define EC_TASK_ETHERCAT_SLAVE_SERVO_H

#include <vector>
#include "EcTaskEthercatSlaveServoBase.h"
#include "EcSlaveDenNetE.h"
#include "EcSlaveElmoDrive.h"
#include "EcSlaveIpos2401MxCat.h"
#include "EcSlavePitchDrive.h"

class EcTaskEthercatSlaveServo
{
public:
    EcTaskEthercatSlaveServo();

    ~EcTaskEthercatSlaveServo();

    EC_T_DWORD addAllTaskSlave();

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
    std::vector<EcTaskEthercatSlaveServoBase*> m_ecTaskEthercatSlaveServoBaseVector;
    int m_numTaskEthercatServoSlave = 0;
    
    EC_T_DWORD addTaskSlaveServo(EcTaskEthercatSlaveServoBase* pSlave);
};

#endif // EC_TASK_ETHERCAT_SLAVE_SERVO_H
