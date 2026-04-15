#ifndef EC_STATE_DISABLED_H
#define EC_STATE_DISABLED_H

class EcStateDisabled
{
public:
    EcStateDisabled();
    ~EcStateDisabled();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_DISABLED_H
