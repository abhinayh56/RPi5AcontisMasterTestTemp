#ifndef EC_STATE_ENABLING_H
#define EC_STATE_ENABLING_H

class EcStateEnabling
{
public:
    EcStateEnabling();
    ~EcStateEnabling();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_ENABLING_H
