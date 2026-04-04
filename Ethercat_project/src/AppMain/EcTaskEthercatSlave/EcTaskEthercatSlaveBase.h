#ifndef EC_TASK_ETHERCAT_SLAVE_BASE_H
#define EC_TASK_ETHERCAT_SLAVE_BASE_H

#include "EcType.h"
#include <string>
#include "ITC.h"

class EcTaskEthercatSlaveBase
{
public:
	EcTaskEthercatSlaveBase(uint16_t ecTaskEthercatId, const std::string &ecTaskEthercatName);

	virtual ~EcTaskEthercatSlaveBase();

	virtual EC_T_DWORD checkSlave() = 0;

	virtual EC_T_DWORD registerTxPdo() = 0;

	virtual EC_T_DWORD registerRxPdo() = 0;

    virtual EC_T_DWORD registerPublisher() = 0;

    virtual EC_T_DWORD registerSubscriber() = 0;

	virtual EC_T_DWORD transferTxPdo() = 0;

	virtual EC_T_DWORD transferRxPdo() = 0;

	virtual EC_T_DWORD processTxPdo() = 0;

	virtual EC_T_DWORD processRxPdo() = 0;

	virtual EC_T_DWORD publishData() = 0;

	virtual EC_T_DWORD subscribeData() = 0;

	virtual EC_T_DWORD mainProcess() = 0;

	virtual void dispTxPdo() = 0;

	virtual void dispRxPdo() = 0;

	uint16_t getTaskId();

	std::string& getTaskName();

	virtual uint16_t getSlaveAddress() = 0;

	virtual std::string& getSlaveName() = 0;

protected:
	uint16_t m_ecTaskEthercatId;
	std::string m_ecTaskEthercatName;
};

#endif // EC_TASK_ETHERCAT_SLAVE_BASE_H
