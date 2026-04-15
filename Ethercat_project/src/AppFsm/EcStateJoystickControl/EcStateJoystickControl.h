#ifndef EC_STATE_JOYSTICKCONTROL_H
#define EC_STATE_JOYSTICKCONTROL_H

class EcStateJoystickControl
{
public:
    EcStateJoystickControl();
    ~EcStateJoystickControl();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_JOYSTICKCONTROL_H
