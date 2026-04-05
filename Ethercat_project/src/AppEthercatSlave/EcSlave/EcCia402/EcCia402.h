#ifndef EC_CIA402_H
#define EC_CIA402_H

#include "EcSlaveBase.h"

namespace EcCia402Data
{
    enum Cia402Mode : int8_t
    {
        MODE_NO_MODE,
        MODE_PROFILE_POSITION = 1,
        MODE_PROFILE_VELOCITY = 2,
        MODE_PROFILE_TORQUE = 3,
        MODE_HOMING = 6,
        MODE_INTERPOLATED_POSITION = 7,
        MODE_CYCLIC_POSITION = 8,
        MODE_CYCLIC_VELOCITY = 9,
        MODE_CYCLIC_TORQUE = 10
    };
}

class EcCia402 : public EcSlaveBase
{
public:
    EcCia402(uint16_t slaveAddr, const std::string &slaveName);

    virtual ~EcCia402();

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

    virtual EC_T_DWORD checkFault();

    virtual EC_T_DWORD clearFault();

    virtual EC_T_DWORD enable();

    virtual EC_T_DWORD disable();

    virtual EC_T_DWORD setModeOfOperation(EcCia402Data::Cia402Mode mode);

    virtual EC_T_DWORD setTargetPosition(int32_t targetPosition);

    virtual EC_T_DWORD setTargetVelocity(int32_t targetVelocity);
    
    virtual EC_T_DWORD setTargetTorque(int16_t targetTorque);

    virtual EC_T_DWORD syncPosition();

    virtual EC_T_DWORD syncVelocity();

    virtual EC_T_DWORD syncTorque();
};

#endif // EC_CIA402_H
