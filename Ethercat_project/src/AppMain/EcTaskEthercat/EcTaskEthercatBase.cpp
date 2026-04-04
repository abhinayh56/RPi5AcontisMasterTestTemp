#include "EcTaskEthercatBase.h"

EcTaskEthercatBase::EcTaskEthercatBase()
{
}

virtual EcTaskEthercatBase::~EcTaskEthercatBase() {}

EC_T_DWORD EcTaskEthercatBase::checkSlave()
{
    return 0;
}

EC_T_DWORD EcTaskEthercatBase::registerTxPdo() { return 0; }

EC_T_DWORD EcTaskEthercatBase::registerRxPdo() { return 0; }

EC_T_DWORD EcTaskEthercatBase::registerPublisher() { return 0; }

EC_T_DWORD EcTaskEthercatBase::registerSubscriber() { return 0; }

EC_T_DWORD EcTaskEthercatBase::transferTxPdo() { return 0; }

EC_T_DWORD EcTaskEthercatBase::transferRxPdo() { return 0; }

EC_T_DWORD EcTaskEthercatBase::processTxPdo() { return 0; }

EC_T_DWORD EcTaskEthercatBase::processRxPdo() { return 0; }

EC_T_DWORD EcTaskEthercatBase::publishData() { return 0; }

EC_T_DWORD EcTaskEthercatBase::subscribeData() { return 0; }

EC_T_DWORD EcTaskEthercatBase::mainProcess() { return 0; }

void EcTaskEthercatBase::dispTxPdo() {}

void EcTaskEthercatBase::dispRxPdo() {}

uint16_t EcTaskEthercatBase::getAddress()
{
}

std::string& EcTaskEthercatBase::getName()
{
}
