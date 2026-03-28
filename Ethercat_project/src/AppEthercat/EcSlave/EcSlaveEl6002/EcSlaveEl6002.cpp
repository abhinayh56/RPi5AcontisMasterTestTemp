#include "EcSlaveEl6002.h"

EcSlaveEl6002::EcSlaveEl6002(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveEl6002::~EcSlaveEl6002()
{
}

EC_T_DWORD EcSlaveEl6002::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Status);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_0);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_2);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_3);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_4);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_5);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_6);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_7);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_8);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_9);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_10);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_11);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_12);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_13);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_14);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_15);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_16);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_17);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_18);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_19);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_20);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_21);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Status);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_0);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_2);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_3);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_4);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_5);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_6);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_7);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_8);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_9);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_10);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_11);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_12);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_13);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_14);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_15);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_16);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_17);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_18);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_19);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_20);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_21);

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Ctrl);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_0);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_2);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_3);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_4);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_5);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_6);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_7);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_8);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_9);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_10);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_11);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_12);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_13);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_14);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_15);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_16);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_17);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_18);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_19);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_20);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_21);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Ctrl);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_0);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_2);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_3);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_4);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_5);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_6);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_7);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_8);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_9);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_10);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_11);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_12);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_13);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_14);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_15);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_16);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_17);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_18);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_19);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_20);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_21);

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Channel_1_Status, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_0, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_1, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_2, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_3, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_4, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_5, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_6, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_7, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_8, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_9, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_10, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_11, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_12, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_13, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_14, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_15, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_16, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_17, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_18, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_19, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_20, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_21, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Status, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_0, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_1, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_2, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_3, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_4, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_5, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_6, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_7, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_8, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_9, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_10, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_11, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_12, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_13, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_14, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_15, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_16, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_17, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_18, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_19, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_20, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_21, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Channel_1_Ctrl, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_0, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_1, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_2, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_3, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_4, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_5, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_6, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_7, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_8, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_9, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_10, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_11, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_12, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_13, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_14, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_15, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_16, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_17, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_18, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_19, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_20, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_21, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Ctrl, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_0, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_1, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_2, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_3, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_4, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_5, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_6, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_7, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_8, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_9, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_10, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_11, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_12, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_13, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_14, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_15, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_16, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_17, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_18, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_19, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_20, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_21, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveEl6002::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveEl6002::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1 (Status, Data_In_0_21): " << m_txPdo.Channel_1_Status.value << ", " <<
	m_txPdo.Channel_1_Data_In_0.value << ", " <<
	m_txPdo.Channel_1_Data_In_1.value << ", " <<
	m_txPdo.Channel_1_Data_In_2.value << ", " <<
	m_txPdo.Channel_1_Data_In_3.value << ", " <<
	m_txPdo.Channel_1_Data_In_4.value << ", " <<
	m_txPdo.Channel_1_Data_In_5.value << ", " <<
	m_txPdo.Channel_1_Data_In_6.value << ", " <<
	m_txPdo.Channel_1_Data_In_7.value << ", " <<
	m_txPdo.Channel_1_Data_In_8.value << ", " <<
	m_txPdo.Channel_1_Data_In_9.value << ", " <<
	m_txPdo.Channel_1_Data_In_10.value << ", " <<
	m_txPdo.Channel_1_Data_In_11.value << ", " <<
	m_txPdo.Channel_1_Data_In_12.value << ", " <<
	m_txPdo.Channel_1_Data_In_13.value << ", " <<
	m_txPdo.Channel_1_Data_In_14.value << ", " <<
	m_txPdo.Channel_1_Data_In_15.value << ", " <<
	m_txPdo.Channel_1_Data_In_16.value << ", " <<
	m_txPdo.Channel_1_Data_In_17.value << ", " <<
	m_txPdo.Channel_1_Data_In_18.value << ", " <<
	m_txPdo.Channel_1_Data_In_19.value << ", " <<
	m_txPdo.Channel_1_Data_In_20.value << ", " <<
	m_txPdo.Channel_1_Data_In_21.value << ", " <<
	"Channel_2 (Status, Data_In_0_21): " << m_txPdo.Channel_2_Status.value << ", " <<
	m_txPdo.Channel_2_Data_In_0.value << ", " <<
	m_txPdo.Channel_2_Data_In_1.value << ", " <<
	m_txPdo.Channel_2_Data_In_2.value << ", " <<
	m_txPdo.Channel_2_Data_In_3.value << ", " <<
	m_txPdo.Channel_2_Data_In_4.value << ", " <<
	m_txPdo.Channel_2_Data_In_5.value << ", " <<
	m_txPdo.Channel_2_Data_In_6.value << ", " <<
	m_txPdo.Channel_2_Data_In_7.value << ", " <<
	m_txPdo.Channel_2_Data_In_8.value << ", " <<
	m_txPdo.Channel_2_Data_In_9.value << ", " <<
	m_txPdo.Channel_2_Data_In_10.value << ", " <<
	m_txPdo.Channel_2_Data_In_11.value << ", " <<
	m_txPdo.Channel_2_Data_In_12.value << ", " <<
	m_txPdo.Channel_2_Data_In_13.value << ", " <<
	m_txPdo.Channel_2_Data_In_14.value << ", " <<
	m_txPdo.Channel_2_Data_In_15.value << ", " <<
	m_txPdo.Channel_2_Data_In_16.value << ", " <<
	m_txPdo.Channel_2_Data_In_17.value << ", " <<
	m_txPdo.Channel_2_Data_In_18.value << ", " <<
	m_txPdo.Channel_2_Data_In_19.value << ", " <<
	m_txPdo.Channel_2_Data_In_20.value << ", " <<
	m_txPdo.Channel_2_Data_In_21.value
	<< std::endl;
}

void EcSlaveEl6002::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1 (Ctrl, Data_Out_0_21): " << m_rxPdo.Channel_1_Ctrl.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_0.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_1.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_2.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_3.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_4.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_5.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_6.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_7.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_8.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_9.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_10.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_11.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_12.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_13.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_14.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_15.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_16.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_17.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_18.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_19.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_20.value << ", " <<
	m_rxPdo.Channel_1_Data_Out_21.value << ", " <<
	"Channel_2 (Ctrl, Data_Out_0_21): " << m_rxPdo.Channel_2_Ctrl.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_0.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_1.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_2.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_3.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_4.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_5.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_6.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_7.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_8.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_9.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_10.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_11.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_12.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_13.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_14.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_15.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_16.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_17.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_18.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_19.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_20.value << ", " <<
	m_rxPdo.Channel_2_Data_Out_21.value
	<< std::endl;
}
