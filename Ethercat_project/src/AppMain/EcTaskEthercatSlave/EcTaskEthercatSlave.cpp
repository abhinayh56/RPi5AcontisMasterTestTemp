#include "EcTaskEthercatSlave.h"

EcTaskEthercatSlave::EcTaskEthercatSlave()
{
//	m_EcTaskEthercatSlaveBaseVector.resize(12);
}

EcTaskEthercatSlave::~EcTaskEthercatSlave()
{
}

EC_T_DWORD EcTaskEthercatSlave::addSlave(EcTaskEthercatSlaveBase* pSlave)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_EcTaskEthercatSlaveBaseVector.push_back(pSlave);
    m_numSlaves = m_EcTaskEthercatSlaveBaseVector.size();
    std::cout << "m_EcTaskEthercatSlaveBaseVector.size(): " << m_EcTaskEthercatSlaveBaseVector.size() << std::endl;

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::cleanupTask()
{
	std::cout << "Cleanup slaves from memory start\n";
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
    	std::cout << "Cleanup slave " << i + 1 << " from memory\n";
        delete m_EcTaskEthercatSlaveBaseVector[i];
    }

    std::cout << "Cleanup slaves from memory complete\n";

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::configTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= checkSlave();
    dwRes |= registerPdo();
    dwRes |= registerPublisher();
    dwRes |= registerSubscriber();

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::cyclicTask()
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

EC_T_DWORD EcTaskEthercatSlave::checkSlave()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        std::cout << "Checking slave presence at " << m_EcTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_EcTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->checkSlave();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::registerPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
    	std::cout << "Registering TxPdo for " << m_EcTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_EcTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->registerTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_EcTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_EcTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->registerRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::transferTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->transferTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::transferRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->transferRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::processTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->processTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::processRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->processRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->registerPublisher();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->registerSubscriber();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_EcTaskEthercatSlaveBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcTaskEthercatSlave::dispTxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_EcTaskEthercatSlaveBaseVector[i]->dispTxPdo();
    }
}

void EcTaskEthercatSlave::dispRxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_EcTaskEthercatSlaveBaseVector[i]->dispRxPdo();
    }
}
