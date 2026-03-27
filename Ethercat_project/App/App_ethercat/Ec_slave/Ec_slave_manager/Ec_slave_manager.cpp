#include "Ec_slave_manager.h"

Ec_slave_manager::Ec_slave_manager()
{
}

Ec_slave_manager::~Ec_slave_manager()
{
}

EC_T_DWORD Ec_slave_manager::addSlave(Ec_slave_base* pSlave)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_slaveVector.push_back(pSlave);
    m_numSlaves = m_slaveVector.size();

    return dwRes;
}

EC_T_DWORD Ec_slave_manager::cleanup()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        delete m_slaveVector[i];
    }

    return dwRes;
}

EC_T_DWORD Ec_slave_manager::registerPdo()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numSlaves; i++)
    {
        dwRes |= m_slaveVector[i]->registerTxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }

        dwRes |= m_slaveVector[i]->registerRxPdo();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD Ec_slave_manager::cyclicProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    transferTxPdo();
    dispTxPdo();
    processTxPdo();
    publishData();
    subscribeData();
    mainProcess();
    processRxPdo();
    dispRxPdo();
    transferRxPdo();

    return dwRes;
}

EC_T_DWORD Ec_slave_manager::transferTxPdo()
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

EC_T_DWORD Ec_slave_manager::transferRxPdo()
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

EC_T_DWORD Ec_slave_manager::processTxPdo()
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

EC_T_DWORD Ec_slave_manager::processRxPdo()
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

EC_T_DWORD Ec_slave_manager::publishData()
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

EC_T_DWORD Ec_slave_manager::subscribeData()
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

EC_T_DWORD Ec_slave_manager::mainProcess()
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

void Ec_slave_manager::dispTxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_slaveVector[i]->dispTxPdo();
    }
}

void Ec_slave_manager::dispRxPdo()
{
    for(int i = 0; i < m_numSlaves; i++)
    {
        m_slaveVector[i]->dispRxPdo();
    }
}
