// #ifndef EC_TASK_USER_H
// #define EC_TASK_USER_H

// #include <vector>
// #include "EcTaskUserBase.h"

// class EcTaskEthercatSlave
// {
// public:
//     EcTaskEthercatSlave();

//     virtual ~EcTaskEthercatSlave();

//     EC_T_DWORD addAllSlave();

//     EC_T_DWORD cleanupTask();

//     EC_T_DWORD checkSlave();

//     EC_T_DWORD registerPdo();
    
// 	EC_T_DWORD registerPublisher();

// 	EC_T_DWORD registerSubscriber();

//     EC_T_DWORD transferTxPdo();

//     EC_T_DWORD transferRxPdo();

//     EC_T_DWORD processTxPdo();

//     EC_T_DWORD processRxPdo();

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
