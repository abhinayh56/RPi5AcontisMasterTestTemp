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
    dwRes |= ecTaskEthercatSlaveServo.addAllTaskSlave();
    dwRes |= ecTaskRobotControl.addAllTaskRobotControl();
    dwRes |= ecTaskUser.addAllTaskUser();
    dwRes |= ecTaskFsm.addAllState();

    return dwRes;
}

EC_T_DWORD EcTaskManager::preapareTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= ecTaskEthercatSlave.checkSlave();
    dwRes |= ecTaskEthercatSlaveServo.checkSlave();

	dwRes |= ecTaskEthercatSlave.registerPdo();
    dwRes |= ecTaskEthercatSlaveServo.registerPdo();

    dwRes |= ecTaskInterface.registerPublisher();
	dwRes |= ecTaskEthercatSlave.registerPublisher();
    dwRes |= ecTaskEthercatSlaveServo.registerPublisher();

	dwRes |= ecTaskUser.registerPublisher();
    dwRes |= ecTaskRobotControl.registerPublisher();
    dwRes |= ecTaskInterface.registerSubscriber();
	dwRes |= ecTaskEthercatSlave.registerSubscriber();
    dwRes |= ecTaskEthercatSlaveServo.registerSubscriber();

	dwRes |= ecTaskUser.registerSubscriber();
    dwRes |= ecTaskRobotControl.registerSubscriber();

    dwRes |= ecTaskFsm.setTaskAddr({&ecTaskEthercatSlave, &ecTaskEthercatSlaveServo, &ecTaskRobotControl, &ecTaskUser, &ecTaskInterface});
    dwRes |= ecTaskFsm.config();
    
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
    dwRes |= ecTaskEthercatSlaveServo.transferTxPdo();

//	ecTaskEthercatSlave.dispTxPdo();
	dwRes |= ecTaskEthercatSlave.processTxPdo();
    dwRes |= ecTaskEthercatSlaveServo.processTxPdo();

	dwRes |= ecTaskEthercatSlave.publishData();
    dwRes |= ecTaskEthercatSlaveServo.publishData();


    dwRes |= ecTaskInterface.subscribeData();

    dwRes |= ecTaskFsm.update();

    dwRes |= ecTaskUser.subscribeData();
    dwRes |= ecTaskUser.mainProcess();
    dwRes |= ecTaskUser.publishData();

    dwRes |= ecTaskRobotControl.subscribeData();
    dwRes |= ecTaskRobotControl.mainProcess();
    dwRes |= ecTaskRobotControl.publishData();

    dwRes |= ecTaskInterface.publishData();

	dwRes |= ecTaskEthercatSlave.subscribeData();
    dwRes |= ecTaskEthercatSlaveServo.subscribeData();

	dwRes |= ecTaskEthercatSlave.mainProcess();
    dwRes |= ecTaskEthercatSlaveServo.mainProcess();

	dwRes |= ecTaskEthercatSlave.processRxPdo();
    dwRes |= ecTaskEthercatSlaveServo.processRxPdo();

//	ecTaskEthercatSlave.dispRxPdo();
	dwRes |= ecTaskEthercatSlave.transferRxPdo();
    dwRes |= ecTaskEthercatSlaveServo.transferRxPdo();


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
