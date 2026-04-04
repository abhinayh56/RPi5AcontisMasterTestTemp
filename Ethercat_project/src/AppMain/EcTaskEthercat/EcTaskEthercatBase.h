#ifndef EC_TASK_ETHERCAT_BASE_H
#define EC_TASK_ETHERCAT_BASE_H

#include "EcType.h"
#include <string>
#include "ITC.h"

class EcTaskEthercatBase
{
public:
	EcTaskEthercatBase(uint16_t ecTaskEthercatId, const std::string &ecTaskEthercatName);

	virtual ~EcTaskEthercatBase();

	virtual EC_T_DWORD checkSlave();

	virtual EC_T_DWORD registerTxPdo();

	virtual EC_T_DWORD registerRxPdo();

    virtual EC_T_DWORD registerPublisher();

    virtual EC_T_DWORD registerSubscriber();

	virtual EC_T_DWORD transferTxPdo();

	virtual EC_T_DWORD transferRxPdo();

	virtual EC_T_DWORD processTxPdo();

	virtual EC_T_DWORD processRxPdo();

	virtual EC_T_DWORD publishData();

	virtual EC_T_DWORD subscribeData();

	virtual EC_T_DWORD mainProcess();

	virtual void dispTxPdo();

	virtual void dispRxPdo();

	virtual uint16_t getAddress();

	virtual std::string& getName();

private:
	uint16_t m_ecTaskEthercatId;
	std::string m_ecTaskEthercatName;
};

#endif // EC_TASK_ETHERCAT_BASE_H
