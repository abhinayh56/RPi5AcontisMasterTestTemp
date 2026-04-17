#ifndef EC_SLAVE_EL2008_H
#define EC_SLAVE_EL2008_H

#include "EcSlaveBase.h"

namespace EcSlaveEl2008Data
{
	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x7000, 1, bool, 1> Channel_1;
        PdoVariable<0x7010, 1, bool, 1> Channel_2;
        PdoVariable<0x7020, 1, bool, 1> Channel_3;
        PdoVariable<0x7030, 1, bool, 1> Channel_4;
        PdoVariable<0x7040, 1, bool, 1> Channel_5;
        PdoVariable<0x7050, 1, bool, 1> Channel_6;
        PdoVariable<0x7060, 1, bool, 1> Channel_7;
        PdoVariable<0x7070, 1, bool, 1> Channel_8;
	};
	#pragma pack(pop)
}

class EcSlaveEl2008 : public EcSlaveBase
{
public:
	EcSlaveEl2008(uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl2008();

	virtual EC_T_DWORD registerTxPdo() override;

	virtual EC_T_DWORD registerRxPdo() override;

	virtual EC_T_DWORD transferTxPdo() override;

	virtual EC_T_DWORD transferRxPdo() override;

	virtual EC_T_DWORD processTxPdo() override;

	virtual EC_T_DWORD processRxPdo() override;

	virtual EC_T_DWORD registerPublisher() override;

	virtual EC_T_DWORD registerSubscriber() override;

	virtual EC_T_DWORD publishData() override;

	virtual EC_T_DWORD subscribeData() override;

	virtual EC_T_DWORD mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

protected:
	EcSlaveEl2008Data::RxPdo m_rxPdo;
	uint8_t m_rxPdoValue = 0;
	
private:
	Data_store_element<bool> m_OutputCh_1;
	Data_store_element<bool> m_OutputCh_2;
	Data_store_element<bool> m_OutputCh_3;
	Data_store_element<bool> m_OutputCh_4;
	Data_store_element<bool> m_OutputCh_5;
	Data_store_element<bool> m_OutputCh_6;
	Data_store_element<bool> m_OutputCh_7;
	Data_store_element<bool> m_OutputCh_8;
};

#endif // EC_SLAVE_EL2008_H
