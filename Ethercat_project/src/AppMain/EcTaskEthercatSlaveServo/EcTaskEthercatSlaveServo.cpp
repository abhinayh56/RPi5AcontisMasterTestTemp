#include "EcTaskEthercatSlaveServo.h"

EcTaskEthercatSlaveServo::EcTaskEthercatSlaveServo()
{
//	m_ecTaskEthercatSlaveServoBaseVector.resize(12);
}

EcTaskEthercatSlaveServo::~EcTaskEthercatSlaveServo()
{
}

EC_T_DWORD EcTaskEthercatSlaveServo::addAllTaskSlave()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= addTaskSlaveServo(new EcSlavePitchDrive(1011, "motor_1"));
	dwRes |= addTaskSlaveServo(new EcSlavePitchDrive(1012, "motor_2"));
	dwRes |= addTaskSlaveServo(new EcSlavePitchDrive(1013, "motor_3"));
	dwRes |= addTaskSlaveServo(new EcSlavePitchDrive(1014, "motor_4"));
	dwRes |= addTaskSlaveServo(new EcSlaveDenNetE(1007, "den_net_e"));

	return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::cleanupTask()
{
	std::cout << "Cleanup slaves from memory start\n";
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
    	std::cout << "Cleanup slave " << i + 1 << " from memory\n";
        delete m_ecTaskEthercatSlaveServoBaseVector[i];
    }

    std::cout << "Cleanup slaves from memory complete\n";

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::checkSlave()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        std::cout << "Checking slave presence at " << m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->checkSlave();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::registerPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
    	std::cout << "Registering TxPdo for " << m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerPublisher();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerSubscriber();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::transferTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->transferTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::transferRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->transferRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::processTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->processTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::processRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->processRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlaveServo::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcTaskEthercatSlaveServo::dispTxPdo()
{
    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        m_ecTaskEthercatSlaveServoBaseVector[i]->dispTxPdo();
    }
}

void EcTaskEthercatSlaveServo::dispRxPdo()
{
    for(int i = 0; i < m_numTaskEthercatServoSlave; i++)
    {
        m_ecTaskEthercatSlaveServoBaseVector[i]->dispRxPdo();
    }
}

EC_T_DWORD EcTaskEthercatSlaveServo::addTaskSlaveServo(EcTaskEthercatSlaveServoBase* pSlave)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_ecTaskEthercatSlaveServoBaseVector.push_back(pSlave);
    m_numTaskEthercatServoSlave = m_ecTaskEthercatSlaveServoBaseVector.size();
    std::cout << "m_ecTaskEthercatSlaveServoBaseVector.size(): " << m_numTaskEthercatServoSlave << std::endl;

    return dwRes;
}
