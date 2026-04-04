// #ifndef EC_TASK_USER_H
// #define EC_TASK_USER_H

// #include <vector>
// #include "EcTaskUserBase.h"

// class EcTaskUser
// {
// public:
//     EcTaskUser();

//     virtual ~EcTaskUser();

//     EC_T_DWORD addAllUserTask();

//     EC_T_DWORD cleanupTask();
    
// 	EC_T_DWORD registerPublisher();

// 	EC_T_DWORD registerSubscriber();

//     EC_T_DWORD publishData();

//     EC_T_DWORD subscribeData();

//     EC_T_DWORD mainProcess();

//     void dispTxPdo();

//     void dispRxPdo();

// private:
//     std::vector<EcTaskUserBase*> m_EcTaskUserBaseVector;
//     int m_numEcTaskUser = 0;

//     EC_T_DWORD addUserTask(EcTaskUserBase* pSlave);

// #endif // EC_TASK_USER_H
