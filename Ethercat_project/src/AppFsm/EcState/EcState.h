#ifndef EC_STATE_H
#define EC_STATE_H

namespace EcStateData
{
    enum StateId : uint32_t
    {
        STANDBY = 101,
        FAULT = 102,
        CLEARING_FAULT = 103,
        INITIALIZING = 104,
        INITIALIZED = 105,
        DISABLING = 106,
        DISABLED = 107,
        ENABLING = 108,
        ENABLED = 109,
        READY = 110,
        JOYSTICKCONTROL = 111
    };
}

#endif // EC_STATE_H
