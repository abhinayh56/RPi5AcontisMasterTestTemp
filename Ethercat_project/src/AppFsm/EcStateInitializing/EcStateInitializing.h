#ifndef EC_STATE_INITIALIZING_H
#define EC_STATE_INITIALIZING_H

class EcStateInitializing
{
public:
    EcStateInitializing();
    ~EcStateInitializing();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_INITIALIZING_H
