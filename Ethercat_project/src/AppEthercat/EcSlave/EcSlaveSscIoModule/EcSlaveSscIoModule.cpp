#include "EcSlaveSscIoModule.h"

EcSlaveSscIoModule::EcSlaveSscIoModule(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveSscIoModule::~EcSlaveSscIoModule()
{
}

EC_T_DWORD EcSlaveSscIoModule::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.footpedal_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.footpedal_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.QUALITY_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.QUALITY_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.GRIP_VAL_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.GRIP_VAL_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.X_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Y_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Z_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q1_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q2_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q3_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q4_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.X_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Y_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Z_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q1_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q2_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q3_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q4_RIGHT);

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.IPC_TX_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.IPC_RX_1);

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.footpedal_1, pBuffer);
	transferInputPdoObject(m_txPdo.footpedal_1, pBuffer);
	transferInputPdoObject(m_txPdo.QUALITY_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.QUALITY_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.GRIP_VAL_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.GRIP_VAL_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.X_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Y_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Z_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q1_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q2_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q3_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q4_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.X_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Y_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Z_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q1_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q2_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q3_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q4_RIGHT, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.IPC_TX_1, pBuffer);
	transferOutputPdoObject(m_rxPdo.IPC_RX_1, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveSscIoModule::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveSscIoModule::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Device_ID: " << m_txPdo.footpedal_1.value << ", "
	"Second: " << m_txPdo.footpedal_1.value << ", "
	"Minute: " << m_txPdo.QUALITY_LEFT.value << ", "
	"Hour: " << m_txPdo.QUALITY_RIGHT.value << ", "
	"Day: " << m_txPdo.GRIP_VAL_RIGHT.value << ", "
	"Month: " << m_txPdo.GRIP_VAL_LEFT.value << ", "
	"Year: " << m_txPdo.X_LEFT.value << ", "
	"Roll_Offset: " << m_txPdo.Y_LEFT.value << ", "
	"Pitch_Offset: " << m_txPdo.Z_LEFT.value << ", "
	"Yaw_Offset: " << m_txPdo.Q1_LEFT.value << ", "
	"Grip_Offset: " << m_txPdo.Q2_LEFT.value << ", "
	"No_of_Usages: " << m_txPdo.Q3_LEFT.value << ", "
	"Max_Usages: " << m_txPdo.Q4_LEFT.value << ", "
	"Digital_Inputs: " << m_txPdo.X_RIGHT.value << ", "
	"Grip_Counts: " << m_txPdo.Y_RIGHT.value << ", "
	"System_Number: " << m_txPdo.Z_RIGHT.value << ", "
	"Device_UID: " << m_txPdo.Q1_RIGHT.value << ", "
	"Spare_Bytes: " << m_txPdo.Q2_RIGHT.value << ", "
	"MFG_Day: " << m_txPdo.Q3_RIGHT.value << ", "
	"MFG_Year: " << m_txPdo.Q4_RIGHT.value
	<< std::endl;
}

void EcSlaveSscIoModule::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Acknowledge: " << m_rxPdo.IPC_TX_1.value << ", "
	"Grip_Offset: " << m_rxPdo.IPC_RX_1.value
	<< std::endl;
}
