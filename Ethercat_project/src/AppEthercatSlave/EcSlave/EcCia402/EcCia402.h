#ifndef EC_CIA402_H
#define EC_CIA402_H

#include "EcSlaveBase.h"

namespace EcCia402Data
{
    namespace Object
    {
        namespace ModeOfOperation
        {
            enum Value : int8_t
            {
                COMMUTATION_OFFSET_DETECTION = -2,
                COGGING_COMPENSATION_RECORDING = -1,
                NONE = 0,
                PROFILE_POSITION = 1,
                VELOCITY = 2,
                PROFILE_VELOCITY = 3,
                PROFILE_TORQUE = 4,
                HOMING = 6,
                INTERPOLATED_POSITION = 7,
                CYCLIC_SYNC_POSITION = 8,
                CYCLIC_SYNC_VELOCITY = 9,
                CYCLIC_SYNC_TORQUE = 10
            };
        }

        namespace ModeOfOperationDisplay
        {
            enum Value : int8_t
            {
                COMMUTATION_OFFSET_DETECTION = -2,
                COGGING_COMPENSATION_RECORDING = -1,
                NONE = 0,
                PROFILE_POSITION = 1,
                VELOCITY = 2,
                PROFILE_VELOCITY = 3,
                PROFILE_TORQUE = 4,
                HOMING = 6,
                INTERPOLATED_POSITION = 7,
                CYCLIC_SYNC_POSITION = 8,
                CYCLIC_SYNC_VELOCITY = 9,
                CYCLIC_SYNC_TORQUE = 10
            };
        }

        namespace ControlWord
        {
            enum BitMask : uint16_t
            {
                SHUT_DOWN = (1 << 2) | (1 << 1),
                SWITCH_ON = (1 << 2) | (1 << 1) | (1 << 0),
                SWITCH_ON_AND_ENABLE_OPERATION = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),
                DISABLE_VOLTAGE = 0,
                QUICK_STOP = (1 << 1),
                DISABLE_OPERATION = (1 << 2) | (1 << 1) | (1 << 0),
                ENABLE_OPERATION = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),
                FAULT_RESET = (1 << 7)
            };
            namespace BitData
            {
                enum Value : uint16_t
                {
                    SHUT_DOWN = (1 << 2) | (1 << 1),
                    SWITCH_ON = (1 << 2) | (1 << 1) | (1 << 0),
                    SWITCH_ON_AND_ENABLE_OPERATION = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),
                    DISABLE_VOLTAGE = 0,
                    QUICK_STOP = (1 << 1),
                    DISABLE_OPERATION = (1 << 2) | (1 << 1) | (1 << 0),
                    ENABLE_OPERATION = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),
                    FAULT_RESET = (1 << 7)
                };
            }
        }
        
        namespace StatusWord
        {
            enum BitMask : uint16_t
            {
                NOT_READY_TO_SWITCH_ON = 0,
                SWITCH_ON_DISABLED = (1 << 6),
                READY_TO_SWITCH_ON = (1 << 5) | (1 << 0),
                SWITCHED_ON = (1 << 5) | (1 << 1) | (1 << 0),
                OPERATION_ENABLED = (1 << 5) | (1 << 2) | (1 << 1) | (1 << 0),
                QUICK_STOP_ACTIVE = (1 << 2) | (1 << 1) | (1 << 0),
                FAULT_REACTION_ACTIVE = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),
                FAULT = (1 << 3)
            };
            namespace BitData
            {
                enum Value : uint16_t
                {
                    NOT_READY_TO_SWITCH_ON = 0,
                    SWITCH_ON_DISABLED = (1 << 6),
                    READY_TO_SWITCH_ON = (1 << 5) | (1 << 0),
                    SWITCHED_ON = (1 << 5) | (1 << 1) | (1 << 0),
                    OPERATION_ENABLED = (1 << 5) | (1 << 2) | (1 << 1) | (1 << 0),
                    QUICK_STOP_ACTIVE = (1 << 2) | (1 << 1) | (1 << 0),
                    FAULT_REACTION_ACTIVE = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),
                    FAULT = (1 << 3)
                };
            }
        }
    }

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
        int32_t position = 0;
        int32_t velocity = 0;
        int16_t torque = 0;
    };

    struct StatusWord
    {
        uint16_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct ModeOfOperationDisplay
    {
        int8_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct ActualPosition
    {
        int32_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct ActualVelocity
    {
        int32_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct ActualTorque
    {
        int16_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct ControlWord
    {
        uint16_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct ModeOfOperation
    {
    	int8_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct TargetPosition
    {
        int32_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct TargetVelocity
    {
        int32_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct TargetTorque
    {
        int16_t* p_value = nullptr;
        bool* p_isSupported = nullptr;
    };

    struct PdoTx
    {
        StatusWord statusWord;
        ModeOfOperationDisplay modeOfOperationDisplay;
        ActualPosition actualPosition;
        ActualVelocity actualVelocity;
        ActualTorque actualTorque;
    };

    struct PdoRx
    {
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

    virtual EC_T_DWORD setOffsetPosition(int32_t offsetPosition);

    virtual EC_T_DWORD setOffsetVelocity(int32_t offsetVelocity);
    
    virtual EC_T_DWORD setOffsetTorque(int16_t offsetTorque);

    virtual EC_T_DWORD setModeOfOperation(int8_t mode);

    virtual EC_T_DWORD setTargetPosition(int32_t targetPosition);

    virtual EC_T_DWORD setTargetVelocity(int32_t targetVelocity);

    virtual EC_T_DWORD setTargetTorque(int16_t targetTorque);

    virtual EC_T_DWORD syncPosition();

    virtual EC_T_DWORD syncVelocity();

    virtual EC_T_DWORD syncTorque();

protected:
    EcCia402Data::Offset m_offset;
    EcCia402Data::PdoTx m_Cia402PdoTx;
    EcCia402Data::PdoRx m_Cia402PdoRx;
};

#endif // EC_CIA402_H
