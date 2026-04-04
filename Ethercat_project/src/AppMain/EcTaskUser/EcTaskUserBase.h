// #ifndef EC_TASK_USER_BASE_H
// #define EC_TASK_USER_BASE_H

// #include "EcType.h"
// #include "ITC.h"

// class EcTaskUserBase
// {
// public:
//     EcTaskUserBase(uint16_t ecTaskUserBaseId, const std::string &ecTaskUserBaseName);

//     virtual ~EcTaskUserBase();

//     virtual EC_T_DWORD registerPublisher() = 0;

//     virtual EC_T_DWORD registerSubscriber() = 0;

//     virtual EC_T_DWORD mainProcess() = 0;

//     uint16_t getTaskId();

//     std::string &getTaskName();

// protected:
//     uint16_t m_ecTaskUserBaseId;
//     std::string m_ecTaskUserBaseName;
// };

// #endif // EC_TASK_USER_BASE_H
