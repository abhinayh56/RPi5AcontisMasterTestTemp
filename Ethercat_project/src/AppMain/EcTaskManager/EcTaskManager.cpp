#include "EcTaskManager.h"

EcTaskManager::EcTaskManager()
{
}

EcTaskManager::~EcTaskManager()
{
}

EC_T_DWORD EcTaskManager::initTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= ecTaskEthercatSlave.addSlaves();

    return dwRes;
}

EC_T_DWORD EcTaskManager::preapareTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecTaskEthercatSlave.checkSlave();
	dwRes |= ecTaskEthercatSlave.registerPdo();
	dwRes |= ecTaskEthercatSlave.registerPublisher();
	dwRes |= ecTaskEthercatSlave.registerSubscriber();

    return dwRes;
}

EC_T_DWORD EcTaskManager::setupTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskManager::cyclicTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecTaskEthercatSlave.transferTxPdo();
//	ecTaskEthercatSlave.dispTxPdo();
	dwRes |= ecTaskEthercatSlave.processTxPdo();
	dwRes |= ecTaskEthercatSlave.publishData();
	dwRes |= ecTaskEthercatSlave.subscribeData();
	dwRes |= ecTaskEthercatSlave.mainProcess();
	dwRes |= ecTaskEthercatSlave.processRxPdo();
//	ecTaskEthercatSlave.dispRxPdo();
	dwRes |= ecTaskEthercatSlave.transferRxPdo();

    return dwRes;
}

EC_T_DWORD EcTaskManager::diagnosisTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskManager::notifyTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}
