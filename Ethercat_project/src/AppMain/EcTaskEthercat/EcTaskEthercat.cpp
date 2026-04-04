#include "EcTaskEthercat.h"

EcTaskEthercat::EcTaskEthercat()
{
//	m_ecTaskEthercatBaseVector.resize(12);
}

EcTaskEthercat::~EcTaskEthercat()
{
}

EC_T_DWORD EcTaskEthercat::addSlave(EcSlaveBase* pSlave)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_ecTaskEthercatBaseVector.push_back(pSlave);
    m_numSlaves = m_ecTaskEthercatBaseVector.size();
    std::cout << "m_ecTaskEthercatBaseVector.size(): " << m_ecTaskEthercatBaseVector.size() << std::endl;

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::cleanupTask()
{
	std::cout << "Cleanup slaves from memory start\n";
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
    	std::cout << "Cleanup slave " << i + 1 << " from memory\n";
        delete m_ecTaskEthercatBaseVector[i];
    }

    std::cout << "Cleanup slaves from memory complete\n";

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::configTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    dwRes |= checkSlave();
    dwRes |= registerPdo();
    dwRes |= registerPublisher();
    dwRes |= registerSubscriber();

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::cyclicTask()
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

EC_T_DWORD EcTaskEthercat::checkSlave()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        std::cout << "Checking slave presence at " << m_ecTaskEthercatBaseVector[i]->getAddress() << ", " <<m_ecTaskEthercatBaseVector[i]->getName() << std::endl;
        dwRes |= m_ecTaskEthercatBaseVector[i]->checkSlave();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::registerPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
    	std::cout << "Registering TxPdo for " << m_ecTaskEthercatBaseVector[i]->getAddress() << ", " <<m_ecTaskEthercatBaseVector[i]->getName() << std::endl;
        dwRes |= m_ecTaskEthercatBaseVector[i]->registerTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_ecTaskEthercatBaseVector[i]->getAddress() << ", " <<m_ecTaskEthercatBaseVector[i]->getName() << std::endl;
        dwRes |= m_ecTaskEthercatBaseVector[i]->registerRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::transferTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->transferTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::transferRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->transferRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::processTxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->processTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::processRxPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->processRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->registerPublisher();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->registerSubscriber();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskEthercat::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_ecTaskEthercatBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcTaskEthercat::dispTxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_ecTaskEthercatBaseVector[i]->dispTxPdo();
    }
}

void EcTaskEthercat::dispRxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_ecTaskEthercatBaseVector[i]->dispRxPdo();
    }
}
