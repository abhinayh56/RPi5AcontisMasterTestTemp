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

    dwRes |= ecTaskInterface.addAllTaskInterface();
    dwRes |= ecTaskEthercatSlave.addAllTaskSlave();
    dwRes |= ecTaskRobotControl.addAllTaskRobotControl();
    dwRes |= ecTaskUser.addAllTaskUser();

    return dwRes;
}

EC_T_DWORD EcTaskManager::preapareTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecTaskEthercatSlave.checkSlave();
	dwRes |= ecTaskEthercatSlave.registerPdo();
    dwRes |= ecTaskInterface.registerPublisher();
	dwRes |= ecTaskEthercatSlave.registerPublisher();
	dwRes |= ecTaskUser.registerPublisher();
    dwRes |= ecTaskRobotControl.registerPublisher();
    dwRes |= ecTaskInterface.registerSubscriber();
	dwRes |= ecTaskEthercatSlave.registerSubscriber();
	dwRes |= ecTaskUser.registerSubscriber();
    dwRes |= ecTaskRobotControl.registerSubscriber();

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

    dwRes |= ecTaskInterface.subscribeData();

    dwRes |= ecTaskUser.subscribeData();
    dwRes |= ecTaskUser.mainProcess();
    dwRes |= ecTaskUser.publishData();

    dwRes |= ecTaskRobotControl.subscribeData();
    dwRes |= ecTaskRobotControl.mainProcess();
    dwRes |= ecTaskRobotControl.publishData();

    dwRes |= ecTaskInterface.publishData();

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
