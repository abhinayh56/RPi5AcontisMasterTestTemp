#ifndef EC_TASK_ETHERCAT_SLAVE_SERVO_BASE_H
#define EC_TASK_ETHERCAT_SLAVE_SERVO_BASE_H

#include "EcSlaveBase.h"

class EcTaskEthercatSlaveServoBase : public EcSlaveBase
{
public:
    EcTaskEthercatSlaveServoBase(uint16_t slaveAddr, const std::string &slaveName);

    virtual ~EcTaskEthercatSlaveServoBase();

	virtual EC_T_DWORD registerTxPdo() override;

	virtual EC_T_DWORD registerRxPdo() override;

	virtual EC_T_DWORD transferTxPdo() override;

	virtual EC_T_DWORD transferRxPdo() override;

	virtual EC_T_DWORD processTxPdo() override;

	virtual EC_T_DWORD processRxPdo() override;

	virtual EC_T_DWORD registerPublisher() override;

	virtual EC_T_DWORD registerSubscriber() override;

	virtual EC_T_DWORD publishData() override;

	virtual EC_T_DWORD subscribeData() override;

	virtual EC_T_DWORD mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

    virtual EC_T_DWORD checkFault() = 0;

    virtual EC_T_DWORD clearFault() = 0;

    virtual EC_T_DWORD enable() = 0;

    virtual EC_T_DWORD disable() = 0;

    virtual EC_T_DWORD quickStop() = 0;

    virtual EC_T_DWORD emergencyStop() = 0;

    virtual EC_T_DWORD setModePosition() = 0;

    virtual EC_T_DWORD setModeVelocity() = 0;
    
    virtual EC_T_DWORD setModeTorque() = 0;

    virtual EC_T_DWORD setTargetPosition(int32_t targetPosition) = 0;

    virtual EC_T_DWORD setTargetVelocity(int32_t targetVelocity) = 0;

    virtual EC_T_DWORD setTargetTorque(int16_t targetTorque) = 0;

    virtual int32_t getActualPosition() = 0;

    virtual int32_t getActualVelocity() = 0;

    virtual int16_t getActualTorque() = 0;

    virtual EC_T_DWORD syncPosition() = 0;

    virtual EC_T_DWORD syncVelocity() = 0;

    virtual EC_T_DWORD syncTorque() = 0;
};

#endif // EC_TASK_ETHERCAT_SLAVE_SERVO_BASE_H
