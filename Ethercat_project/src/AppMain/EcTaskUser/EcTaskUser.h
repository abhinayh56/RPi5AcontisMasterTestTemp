 #ifndef EC_TASK_USER_H
 #define EC_TASK_USER_H

 #include <vector>
 #include "EcTaskUserBase.h"

 class EcTaskUser
 {
 public:
	EcTaskUser();

	~EcTaskUser();

	EC_T_DWORD addAllTaskUser();

	EC_T_DWORD cleanupTask();

	EC_T_DWORD registerPublisher();

	EC_T_DWORD registerSubscriber();

	EC_T_DWORD publishData();

	EC_T_DWORD subscribeData();

	EC_T_DWORD mainProcess();

 private:
	std::vector<EcTaskUserBase*> m_ecTaskUserBaseVector;
	int m_numTaskUser = 0;

	EC_T_DWORD addTaskUser(EcTaskUserBase* pTaskUser);
 };

 #endif // EC_TASK_USER_H
