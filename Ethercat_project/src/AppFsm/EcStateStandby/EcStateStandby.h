#ifndef EC_STATE_STANDBY_H
#define EC_STATE_STANDBY_H

class EcStateStandby
{
public:
    EcStateStandby();
    ~EcStateStandby();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_STANDBY_H

// #ifndef EC_STATE_STANDBY_H
// #define EC_STATE_STANDBY_H

// #include "EcTaskEthercatSlave.h"
// #include "EcTaskEthercatSlaveServo.h"
// #include "EcTaskRobotControl.h"
// #include "EcTaskUser.h"
// #include "EcTaskInterface.h"

// class EcStateStandby
// {
// public:
//     EcStateStandby(
//         EcTaskEthercatSlave* p_ecTaskEthercatSlave_,
//         EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo_,
//         EcTaskRobotControl* p_ecTaskRobotControl_,
//         EcTaskUser* p_ecTaskUser_,
//         EcTaskInterface* p_ecTaskInterface_);
        
//     ~EcStateStandby();

// private:
//     EcTaskEthercatSlave *p_ecTaskEthercatSlave;
//     EcTaskEthercatSlaveServo *p_ecTaskEthercatSlaveServo;
//     EcTaskRobotControl *p_ecTaskRobotControl;
//     EcTaskUser *p_ecTaskUser;
//     EcTaskInterface *p_ecTaskInterface;
// };

// #endif // EC_STATE_STANDBY_H
