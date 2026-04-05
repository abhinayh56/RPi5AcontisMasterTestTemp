#include "EcTaskInterface.h"

EcTaskInterface::EcTaskInterface()
{
}

EcTaskInterface::~EcTaskInterface()
{
}

EC_T_DWORD EcTaskInterface::addAllTaskInterface()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskInterface::cleanupTask()
{
    std::cout << "Cleanup user task from memory start\n";
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskInterface; i++)
    {
    	std::cout << "Cleanup user task " << i + 1 << " from memory\n";
        delete m_ecTaskInterfaceBaseVector[i];
    }

    std::cout << "Cleanup user task from memory complete\n";

    return dwRes;
}

EC_T_DWORD EcTaskInterface::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskInterface; i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerPublisher();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskInterface::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskInterface; i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerSubscriber();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskInterface::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskInterface; i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskInterface::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskInterface; i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskInterface::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskInterface; i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskInterface::addTaskInterface(EcTaskInterfaceBase *pTaskInterface)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_ecTaskInterfaceBaseVector.push_back(pTaskInterface);
    m_numTaskInterface = m_ecTaskInterfaceBaseVector.size();
    std::cout << "m_ecTaskInterfaceBaseVector.size(): " << m_ecTaskInterfaceBaseVector.size() << std::endl;

    return dwRes;
}
