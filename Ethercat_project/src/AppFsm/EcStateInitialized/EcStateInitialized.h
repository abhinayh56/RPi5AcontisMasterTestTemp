#ifndef EC_STATE_INITIALIZED_H
#define EC_STATE_INITIALIZED_H

class EcStateInitialized
{
public:
    EcStateInitialized();
    ~EcStateInitialized();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_INITIALIZED_H