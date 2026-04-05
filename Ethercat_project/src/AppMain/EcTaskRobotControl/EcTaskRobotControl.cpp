#include "EcTaskRobotControl.h"

EcTaskRobotControl::EcTaskRobotControl()
{
}

EcTaskRobotControl::~EcTaskRobotControl()
{
}

EC_T_DWORD EcTaskRobotControl::addAllTaskRobotControl()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::cleanupTask()
{
    std::cout << "Cleanup robot control task from memory start\n";
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
    	std::cout << "Cleanup robot control task " << i + 1 << " from memory\n";
        delete m_ecTaskRobotControlBaseVector[i];
    }

    std::cout << "Cleanup robot control task from memory complete\n";

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::registerPublisher()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->registerPublisher();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::registerSubscriber()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->registerSubscriber();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::publishData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->publishData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::subscribeData()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->subscribeData();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::mainProcess()
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->mainProcess();
        if(dwRes != EC_E_NOERROR)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EC_T_DWORD EcTaskRobotControl::addTaskRobotControl(EcTaskRobotControlBase* pTaskRobotControl)
{
    EC_T_DWORD dwRes = EC_E_NOERROR;

    m_ecTaskRobotControlBaseVector.push_back(pTaskRobotControl);
    m_numTaskRobotControl = m_ecTaskRobotControlBaseVector.size();
    std::cout << "m_ecTaskRobotControlBaseVector.size(): " << m_ecTaskRobotControlBaseVector.size() << std::endl;

    return dwRes;
}
