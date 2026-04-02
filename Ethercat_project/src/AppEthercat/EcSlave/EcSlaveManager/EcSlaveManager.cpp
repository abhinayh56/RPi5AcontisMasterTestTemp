#include "EcSlaveManager.h"

EcSlaveManager::EcSlaveManager()
{
//	m_slaveVector.resize(12);
}

EcSlaveManager::~EcSlaveManager()
{
}

EC_T_DWORD EcSlaveManager::addSlave(EcSlaveBase* pSlave)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_slaveVector.push_back(pSlave);
    m_numSlaves = m_slaveVector.size();
    std::cout << "m_slaveVector.size(): " << m_slaveVector.size() << std::endl;

    return dwRes;
}

EC_T_DWORD EcSlaveManager::cleanupTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        delete m_slaveVector[i];
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::configTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        std::cout << "Checking slave presence at " << m_slaveVector[i]->getAddress() << ", " <<m_slaveVector[i]->getName() << std::endl;
        dwRes |= m_slaveVector[i]->checkSlave();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    for(int i = 0; i < m_numSlaves; i++)
    {
    	std::cout << "Registering TxPdo for " << m_slaveVector[i]->getAddress() << ", " <<m_slaveVector[i]->getName() << std::endl;
        dwRes |= m_slaveVector[i]->registerTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_slaveVector[i]->getAddress() << ", " <<m_slaveVector[i]->getName() << std::endl;
        dwRes |= m_slaveVector[i]->registerRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::cyclicTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    transferTxPdo();
//    dispTxPdo();
    processTxPdo();
    publishData();
    subscribeData();
    mainProcess();
    processRxPdo();
//    dispRxPdo();
    transferRxPdo();

    return dwRes;
}

EC_T_DWORD EcSlaveManager::transferTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->transferTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::transferRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->transferRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::processTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->processTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::processRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->processRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcSlaveManager::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcSlaveManager::dispTxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_slaveVector[i]->dispTxPdo();
    }
}

void EcSlaveManager::dispRxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_slaveVector[i]->dispRxPdo();
    }
}
