#ifndef EC_STATE_FAULT_H
#define EC_STATE_FAULT_H

class EcStateFault
{
public:
    EcStateFault();
    ~EcStateFault();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_FAULT_H