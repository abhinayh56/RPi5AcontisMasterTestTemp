#include "EcTaskUser.h"

EcTaskUser::EcTaskUser()
{
}

EcTaskUser::~EcTaskUser()
{
}

EC_T_DWORD EcTaskUser::addAllTaskUser()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskUser::cleanupTask()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskUser::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numEcTaskUser; i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->registerPublisher();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskUser::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numEcTaskUser; i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->registerSubscriber();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskUser::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numEcTaskUser; i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskUser::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numEcTaskUser; i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskUser::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numEcTaskUser; i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskUser::addTaskUser(EcTaskUserBase *pTaskUser)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_ecTaskUserBaseVector.push_back(pTaskUser);
    m_numEcTaskUser = m_ecTaskUserBaseVector.size();
    std::cout << "m_ecTaskUserBaseVector.size(): " << m_ecTaskUserBaseVector.size() << std::endl;

    return dwRes;
}
