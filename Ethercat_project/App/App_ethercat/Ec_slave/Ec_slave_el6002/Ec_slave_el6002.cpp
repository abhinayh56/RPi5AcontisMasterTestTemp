#include "Ec_slave_el6002.h"

Ec_slave_el6002::Ec_slave_el6002(uint16_t slaveAddr) : Ec_slave_base(slaveAddr)
{
}

Ec_slave_el6002::~Ec_slave_el6002()
{
}

EC_T_DWORD Ec_slave_el6002::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Status);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_0);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_1);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_2);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_3);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_4);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_5);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_6);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_7);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_8);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_9);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_10);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_11);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_12);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_13);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_14);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_15);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_16);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_17);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_18);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_19);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_20);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_1_Data_In_21);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Status);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_0);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_1);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_2);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_3);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_4);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_5);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_6);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_7);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_8);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_9);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_10);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_11);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_12);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_13);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_14);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_15);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_16);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_17);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_18);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_19);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_20);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Channel_2_Data_In_21);

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Ctrl);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_0);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_1);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_2);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_3);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_4);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_5);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_6);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_7);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_8);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_9);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_10);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_11);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_12);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_13);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_14);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_15);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_16);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_17);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_18);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_19);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_20);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_1_Data_Out_21);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Ctrl);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_0);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_1);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_2);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_3);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_4);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_5);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_6);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_7);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_8);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_9);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_10);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_11);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_12);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_13);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_14);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_15);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_16);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_17);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_18);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_19);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_20);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Channel_2_Data_Out_21);

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.Channel_1_Status, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_0, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_1, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_2, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_3, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_4, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_5, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_6, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_7, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_8, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_9, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_10, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_11, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_12, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_13, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_14, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_15, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_16, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_17, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_18, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_19, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_20, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_1_Data_In_21, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Status, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_0, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_1, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_2, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_3, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_4, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_5, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_6, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_7, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_8, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_9, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_10, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_11, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_12, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_13, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_14, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_15, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_16, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_17, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_18, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_19, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_20, pBuffer);
	transferInputPdoObject(m_TxPdo.Channel_2_Data_In_21, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.Channel_1_Ctrl, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_0, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_1, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_2, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_3, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_4, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_5, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_6, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_7, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_8, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_9, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_10, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_11, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_12, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_13, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_14, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_15, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_16, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_17, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_18, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_19, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_20, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_1_Data_Out_21, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Ctrl, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_0, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_1, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_2, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_3, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_4, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_5, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_6, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_7, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_8, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_9, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_10, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_11, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_12, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_13, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_14, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_15, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_16, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_17, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_18, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_19, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_20, pBuffer);
	transferOutputPdoObject(m_RxPdo.Channel_2_Data_Out_21, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_el6002::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_el6002::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Channel_1 (Status, Data_In_0_21): " << m_TxPdo.Channel_1_Status << ", " <<
	m_TxPdo.Channel_1_Data_In_0 << ", " <<
	m_TxPdo.Channel_1_Data_In_1 << ", " <<
	m_TxPdo.Channel_1_Data_In_2 << ", " <<
	m_TxPdo.Channel_1_Data_In_3 << ", " <<
	m_TxPdo.Channel_1_Data_In_4 << ", " <<
	m_TxPdo.Channel_1_Data_In_5 << ", " <<
	m_TxPdo.Channel_1_Data_In_6 << ", " <<
	m_TxPdo.Channel_1_Data_In_7 << ", " <<
	m_TxPdo.Channel_1_Data_In_8 << ", " <<
	m_TxPdo.Channel_1_Data_In_9 << ", " <<
	m_TxPdo.Channel_1_Data_In_10 << ", " <<
	m_TxPdo.Channel_1_Data_In_11 << ", " <<
	m_TxPdo.Channel_1_Data_In_12 << ", " <<
	m_TxPdo.Channel_1_Data_In_13 << ", " <<
	m_TxPdo.Channel_1_Data_In_14 << ", " <<
	m_TxPdo.Channel_1_Data_In_15 << ", " <<
	m_TxPdo.Channel_1_Data_In_16 << ", " <<
	m_TxPdo.Channel_1_Data_In_17 << ", " <<
	m_TxPdo.Channel_1_Data_In_18 << ", " <<
	m_TxPdo.Channel_1_Data_In_19 << ", " <<
	m_TxPdo.Channel_1_Data_In_20 << ", " <<
	m_TxPdo.Channel_1_Data_In_21 << ", " <<
	"Channel_2 (Status, Data_In_0_21): " << m_TxPdo.Channel_2_Status << ", " <<
	m_TxPdo.Channel_2_Data_In_0 << ", " <<
	m_TxPdo.Channel_2_Data_In_1 << ", " <<
	m_TxPdo.Channel_2_Data_In_2 << ", " <<
	m_TxPdo.Channel_2_Data_In_3 << ", " <<
	m_TxPdo.Channel_2_Data_In_4 << ", " <<
	m_TxPdo.Channel_2_Data_In_5 << ", " <<
	m_TxPdo.Channel_2_Data_In_6 << ", " <<
	m_TxPdo.Channel_2_Data_In_7 << ", " <<
	m_TxPdo.Channel_2_Data_In_8 << ", " <<
	m_TxPdo.Channel_2_Data_In_9 << ", " <<
	m_TxPdo.Channel_2_Data_In_10 << ", " <<
	m_TxPdo.Channel_2_Data_In_11 << ", " <<
	m_TxPdo.Channel_2_Data_In_12 << ", " <<
	m_TxPdo.Channel_2_Data_In_13 << ", " <<
	m_TxPdo.Channel_2_Data_In_14 << ", " <<
	m_TxPdo.Channel_2_Data_In_15 << ", " <<
	m_TxPdo.Channel_2_Data_In_16 << ", " <<
	m_TxPdo.Channel_2_Data_In_17 << ", " <<
	m_TxPdo.Channel_2_Data_In_18 << ", " <<
	m_TxPdo.Channel_2_Data_In_19 << ", " <<
	m_TxPdo.Channel_2_Data_In_20 << ", " <<
	m_TxPdo.Channel_2_Data_In_21
	<< std::endl;
}

void Ec_slave_el6002::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"Channel_1 (Ctrl, Data_Out_0_21): " << m_RxPdo.Channel_1_Ctrl << ", " <<
	m_RxPdo.Channel_1_Data_Out_0 << ", " <<
	m_RxPdo.Channel_1_Data_Out_1 << ", " <<
	m_RxPdo.Channel_1_Data_Out_2 << ", " <<
	m_RxPdo.Channel_1_Data_Out_3 << ", " <<
	m_RxPdo.Channel_1_Data_Out_4 << ", " <<
	m_RxPdo.Channel_1_Data_Out_5 << ", " <<
	m_RxPdo.Channel_1_Data_Out_6 << ", " <<
	m_RxPdo.Channel_1_Data_Out_7 << ", " <<
	m_RxPdo.Channel_1_Data_Out_8 << ", " <<
	m_RxPdo.Channel_1_Data_Out_9 << ", " <<
	m_RxPdo.Channel_1_Data_Out_10 << ", " <<
	m_RxPdo.Channel_1_Data_Out_11 << ", " <<
	m_RxPdo.Channel_1_Data_Out_12 << ", " <<
	m_RxPdo.Channel_1_Data_Out_13 << ", " <<
	m_RxPdo.Channel_1_Data_Out_14 << ", " <<
	m_RxPdo.Channel_1_Data_Out_15 << ", " <<
	m_RxPdo.Channel_1_Data_Out_16 << ", " <<
	m_RxPdo.Channel_1_Data_Out_17 << ", " <<
	m_RxPdo.Channel_1_Data_Out_18 << ", " <<
	m_RxPdo.Channel_1_Data_Out_19 << ", " <<
	m_RxPdo.Channel_1_Data_Out_20 << ", " <<
	m_RxPdo.Channel_1_Data_Out_21 << ", " <<
	"Channel_2 (Ctrl, Data_Out_0_21): " << m_RxPdo.Channel_2_Ctrl << ", " <<
	m_RxPdo.Channel_2_Data_Out_0 << ", " <<
	m_RxPdo.Channel_2_Data_Out_1 << ", " <<
	m_RxPdo.Channel_2_Data_Out_2 << ", " <<
	m_RxPdo.Channel_2_Data_Out_3 << ", " <<
	m_RxPdo.Channel_2_Data_Out_4 << ", " <<
	m_RxPdo.Channel_2_Data_Out_5 << ", " <<
	m_RxPdo.Channel_2_Data_Out_6 << ", " <<
	m_RxPdo.Channel_2_Data_Out_7 << ", " <<
	m_RxPdo.Channel_2_Data_Out_8 << ", " <<
	m_RxPdo.Channel_2_Data_Out_9 << ", " <<
	m_RxPdo.Channel_2_Data_Out_10 << ", " <<
	m_RxPdo.Channel_2_Data_Out_11 << ", " <<
	m_RxPdo.Channel_2_Data_Out_12 << ", " <<
	m_RxPdo.Channel_2_Data_Out_13 << ", " <<
	m_RxPdo.Channel_2_Data_Out_14 << ", " <<
	m_RxPdo.Channel_2_Data_Out_15 << ", " <<
	m_RxPdo.Channel_2_Data_Out_16 << ", " <<
	m_RxPdo.Channel_2_Data_Out_17 << ", " <<
	m_RxPdo.Channel_2_Data_Out_18 << ", " <<
	m_RxPdo.Channel_2_Data_Out_19 << ", " <<
	m_RxPdo.Channel_2_Data_Out_20 << ", " <<
	m_RxPdo.Channel_2_Data_Out_21
	<< std::endl;
}