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

    EC_T_DWORD faultClear();

    EC_T_DWORD isFaultClear();

    EC_T_DWORD quickStop();

    EC_T_DWORD isQuickStop();

    EC_T_DWORD emergencyStop();

    EC_T_DWORD isEmergencyStop();

    EC_T_DWORD enable();

    EC_T_DWORD isEnable();

    EC_T_DWORD disable();

    EC_T_DWORD isDisable();

    EC_T_DWORD setModePosition();

    EC_T_DWORD isModePosition();

    EC_T_DWORD setModeVelocity();

    EC_T_DWORD isModeVelocity();
    
    EC_T_DWORD setModeTorque();

    EC_T_DWORD isModeTorque();

    EC_T_DWORD setTargetPosition(int32_t targetPosition);

    EC_T_DWORD setTargetVelocity(int32_t targetVelocity);

    EC_T_DWORD setTargetTorque(int16_t targetTorque);

    int32_t getActualPosition();

    int32_t getActualVelocity();

    int16_t getActualTorque();

    EC_T_DWORD syncPosition();

    EC_T_DWORD syncVelocity();

    EC_T_DWORD syncTorque();

private:
    std::vector<EcTaskEthercatSlaveServoBase*> m_ecTaskEthercatSlaveServoBaseVector;
    int m_numTaskEthercatServoSlave = 0;
    
    EC_T_DWORD addTaskSlaveServo(EcTaskEthercatSlaveServoBase* pSlave);
};

#endif // EC_TASK_ETHERCAT_SLAVE_SERVO_H
