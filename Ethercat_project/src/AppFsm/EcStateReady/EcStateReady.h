#ifndef EC_STATE_READY_H
#define EC_STATE_READY_H

class EcStateReady
{
public:
    EcStateReady();
    ~EcStateReady();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_READY_H