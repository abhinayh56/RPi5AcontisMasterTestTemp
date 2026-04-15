#ifndef EC_STATE_ENABLED_H
#define EC_STATE_ENABLED_H

class EcStateEnabled
{
public:
    EcStateEnabled();
    ~EcStateEnabled();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_ENABLED_H
