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

    struct Offset
    {
        uint32_t position = 0;
        uint32_t velocity = 0;
        uint32_t torque = 0;
    };

    struct StatusWord
    {
        uint16_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct ModeOfOperationDisplay
    {
        uint8_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct ActualPosition
    {
        int32_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct ActualVelocity
    {
        int32_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct ActualTorque
    {
        int16_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct ControlWord
    {
        uint16_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct ModeOfOperation
    {
    	uint8_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct TargetPosition
    {
        int32_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct TargetVelocity
    {
        int32_t* value = nullptr;
        bool* isSupported = nullptr;
    };

    struct TargetTorque
    {
        int16_t* value = nullptr;
        bool* isSupported = nullptr;
    };


    struct Pdo
    {
        StatusWord statusWord;
        ModeOfOperationDisplay modeOfOperationDisplay;
        ActualPosition actualPosition;
        ActualVelocity actualVelocity;
        ActualTorque actualTorque;

        ControlWord controlWord;
        ModeOfOperation modeOfOperation;
        TargetPosition targetPosition;
        TargetVelocity targetVelocity;
        TargetTorque targetTorque;
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

    virtual EC_T_DWORD setOffsetPosition(uint32_t offsetPosition);

    virtual EC_T_DWORD setOffsetVelocity(uint32_t offsetVelocity);
    
    virtual EC_T_DWORD setOffsetTorque(uint32_t offsetTorque);

    virtual EC_T_DWORD setModeOfOperation(EcCia402Data::Cia402Mode mode);

    virtual EC_T_DWORD setTargetPosition(int32_t targetPosition);

    virtual EC_T_DWORD setTargetVelocity(int32_t targetVelocity);

    virtual EC_T_DWORD setTargetTorque(int16_t targetTorque);

    virtual EC_T_DWORD syncPosition();

    virtual EC_T_DWORD syncVelocity();

    virtual EC_T_DWORD syncTorque();

protected:
    EcCia402Data::Offset m_offset;
    EcCia402Data::Pdo m_pdo;
};

#endif // EC_CIA402_H
