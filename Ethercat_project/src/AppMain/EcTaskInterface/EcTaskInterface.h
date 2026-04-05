 #ifndef EC_TASK_INTERFACE_H
 #define EC_TASK_INTERFACE_H

 #include <vector>
 #include "EcTaskInterfaceBase.h"

 class EcTaskInterface
 {
 public:
	EcTaskInterface();

	~EcTaskInterface();

	EC_T_DWORD addAllTaskInterface();

	EC_T_DWORD cleanupTask();

	EC_T_DWORD registerPublisher();

	EC_T_DWORD registerSubscriber();

	EC_T_DWORD publishData();

	EC_T_DWORD subscribeData();

	EC_T_DWORD mainProcess();

 private:
	std::vector<EcTaskInterfaceBase*> m_ecTaskInterfaceBaseVector;
	int m_numTaskInterface = 0;

	EC_T_DWORD addTaskInterface(EcTaskInterfaceBase* pTaskInterface);
 };

 #endif // EC_TASK_INTERFACE_H
