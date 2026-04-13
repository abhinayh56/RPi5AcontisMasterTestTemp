#include "EcTaskEthercatSlave.h"

EcTaskEthercatSlave::EcTaskEthercatSlave()
{
//	m_ecTaskEthercatSlaveBaseVector.resize(12);
}

EcTaskEthercatSlave::~EcTaskEthercatSlave()
{
}

EC_T_DWORD EcTaskEthercatSlave::addAllTaskSlave()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= addTaskSlave(new EcSlaveEl1008(1002, "el1008"));
	dwRes |= addTaskSlave(new EcSlaveEl2008(1003, "el2008"));
	dwRes |= addTaskSlave(new EcSlaveSscIoModule(1009, "io_module"));
	dwRes |= addTaskSlave(new EcSlaveRfidslave(1010, "rfid"));
	dwRes |= addTaskSlave(new EcSlaveEl6002(1015, "serial"));

	return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::cleanupTask()
{
	std::cout << "Cleanup slaves from memory start\n";
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
    	std::cout << "Cleanup slave " << i + 1 << " from memory\n";
        delete m_ecTaskEthercatSlaveBaseVector[i];
    }

    std::cout << "Cleanup slaves from memory complete\n";

    return dwRes;
}

EC_T_DWORD EcTaskEthercatSlave::checkSlave()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        std::cout << "Checking slave presence at " << m_ecTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->checkSlave();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
    	std::cout << "Registering TxPdo for " << m_ecTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_ecTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerRxPdo();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerPublisher();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerSubscriber();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->transferTxPdo();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->transferRxPdo();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->processTxPdo();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->processRxPdo();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->publishData();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->subscribeData();
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

    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcTaskEthercatSlave::dispTxPdo()
{
    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        m_ecTaskEthercatSlaveBaseVector[i]->dispTxPdo();
    }
}

void EcTaskEthercatSlave::dispRxPdo()
{
    for(int i = 0; i < m_numTaskEthercatSlave; i++)
    {
        m_ecTaskEthercatSlaveBaseVector[i]->dispRxPdo();
    }
}

EC_T_DWORD EcTaskEthercatSlave::addTaskSlave(EcTaskEthercatSlaveBase* pSlave)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_ecTaskEthercatSlaveBaseVector.push_back(pSlave);
    m_numTaskEthercatSlave = m_ecTaskEthercatSlaveBaseVector.size();
    std::cout << "m_ecTaskEthercatSlaveBaseVector.size(): " << m_ecTaskEthercatSlaveBaseVector.size() << std::endl;

    return dwRes;
}
