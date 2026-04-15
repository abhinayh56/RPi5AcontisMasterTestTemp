#ifndef EC_STATE_CLEARING_FAULT_H
#define EC_STATE_CLEARING_FAULT_H

class EcStateClearing_Fault
{
public:
    EcStateClearing_Fault();
    ~EcStateClearing_Fault();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_CLEARING_FAULT_H
