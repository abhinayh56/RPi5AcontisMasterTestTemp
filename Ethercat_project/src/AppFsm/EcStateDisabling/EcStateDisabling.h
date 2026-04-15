#ifndef EC_STATE_DISABLING_H
#define EC_STATE_DISABLING_H

class EcStateDisabling
{
public:
    EcStateDisabling();
    ~EcStateDisabling();
    uint32_t config() override;
    uint32_t callback() override;
};

#endif // EC_STATE_DISABLING_H
