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

    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlaveEl1008(1002, "el1008"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlaveEl2008(1003, "el2008"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlaveSscIoModule(1008, "io_module"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlaveRfidslave(1009, "rfid"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlavePitchDrive(1010, "motor_1"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlavePitchDrive(1011, "motor_2"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlavePitchDrive(1012, "motor_3"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlavePitchDrive(1013, "motor_4"));
    dwRes |= ecTaskEthercatSlave.addSlave(new EcSlaveEl6002(1014, "serial"));

    return dwRes;
}

EC_T_DWORD EcTaskManager::preapareTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= ecTaskEthercatSlave.configTask();

    return dwRes;
}

EC_T_DWORD EcTaskManager::setupTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= ecTaskEthercatSlave.configTask();

    return dwRes;
}

EC_T_DWORD EcTaskManager::cyclicTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= ecTaskEthercatSlave.cyclicTask();

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
