#include "Ec_slave_rfidslave.h"

Ec_slave_rfidslave::Ec_slave_rfidslave(uint16_t slaveAddr, const std::string &slaveName) : Ec_slave_base(slaveAddr, slaveName)
{
}

Ec_slave_rfidslave::~Ec_slave_rfidslave()
{
}

EC_T_DWORD Ec_slave_rfidslave::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Device_ID);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Second);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Minute);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Hour);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Day);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Month);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Year);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Roll_Offset);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Pitch_Offset);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Yaw_Offset);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Grip_Offset);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.No_of_Usages);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Max_Usages);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Grip_Counts);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.System_Number);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Device_UID);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.Spare_Bytes);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.MFG_Day);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.MFG_Month);
	dwRes |= lookupInputPdoObject(m_SlaveAddr, m_TxPdo.MFG_Year);

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Acknowledge);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Second);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Minute);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Hour);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Day);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Month);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Year);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.No_of_Usages);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Digital_Outputs);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Grip_Counts);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.System_Number);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Led_Red);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Led_Green);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.LED_Blue);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Spare_Bytes);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Roll_Offset);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Pitch_Offset);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Yaw_Offset);
	dwRes |= lookupOutputPdoObject(m_SlaveAddr, m_RxPdo.Grip_Offset);

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_TxPdo.Device_ID, pBuffer);
	transferInputPdoObject(m_TxPdo.Second, pBuffer);
	transferInputPdoObject(m_TxPdo.Minute, pBuffer);
	transferInputPdoObject(m_TxPdo.Hour, pBuffer);
	transferInputPdoObject(m_TxPdo.Day, pBuffer);
	transferInputPdoObject(m_TxPdo.Month, pBuffer);
	transferInputPdoObject(m_TxPdo.Year, pBuffer);
	transferInputPdoObject(m_TxPdo.Roll_Offset, pBuffer);
	transferInputPdoObject(m_TxPdo.Pitch_Offset, pBuffer);
	transferInputPdoObject(m_TxPdo.Yaw_Offset, pBuffer);
	transferInputPdoObject(m_TxPdo.Grip_Offset, pBuffer);
	transferInputPdoObject(m_TxPdo.No_of_Usages, pBuffer);
	transferInputPdoObject(m_TxPdo.Max_Usages, pBuffer);
	transferInputPdoObject(m_TxPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_TxPdo.Grip_Counts, pBuffer);
	transferInputPdoObject(m_TxPdo.System_Number, pBuffer);
	transferInputPdoObject(m_TxPdo.Device_UID, pBuffer);
	transferInputPdoObject(m_TxPdo.Spare_Bytes, pBuffer);
	transferInputPdoObject(m_TxPdo.MFG_Day, pBuffer);
	transferInputPdoObject(m_TxPdo.MFG_Month, pBuffer);
	transferInputPdoObject(m_TxPdo.MFG_Year, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_RxPdo.Acknowledge, pBuffer);
	transferOutputPdoObject(m_RxPdo.Second, pBuffer);
	transferOutputPdoObject(m_RxPdo.Minute, pBuffer);
	transferOutputPdoObject(m_RxPdo.Hour, pBuffer);
	transferOutputPdoObject(m_RxPdo.Day, pBuffer);
	transferOutputPdoObject(m_RxPdo.Month, pBuffer);
	transferOutputPdoObject(m_RxPdo.Year, pBuffer);
	transferOutputPdoObject(m_RxPdo.No_of_Usages, pBuffer);
	transferOutputPdoObject(m_RxPdo.Digital_Outputs, pBuffer);
	transferOutputPdoObject(m_RxPdo.Grip_Counts, pBuffer);
	transferOutputPdoObject(m_RxPdo.System_Number, pBuffer);
	transferOutputPdoObject(m_RxPdo.Led_Red, pBuffer);
	transferOutputPdoObject(m_RxPdo.Led_Green, pBuffer);
	transferOutputPdoObject(m_RxPdo.LED_Blue, pBuffer);
	transferOutputPdoObject(m_RxPdo.Spare_Bytes, pBuffer);
	transferOutputPdoObject(m_RxPdo.Roll_Offset, pBuffer);
	transferOutputPdoObject(m_RxPdo.Pitch_Offset, pBuffer);
	transferOutputPdoObject(m_RxPdo.Yaw_Offset, pBuffer);
	transferOutputPdoObject(m_RxPdo.Grip_Offset, pBuffer);

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD Ec_slave_rfidslave::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void Ec_slave_rfidslave::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"SLAVE_NAME: " << m_SlaveName << " | " <<
	"Device_ID: " << m_TxPdo.Device_ID.value << ", "
	"Second: " << m_TxPdo.Second.value << ", "
	"Minute: " << m_TxPdo.Minute.value << ", "
	"Hour: " << m_TxPdo.Hour.value << ", "
	"Day: " << m_TxPdo.Day.value << ", "
	"Month: " << m_TxPdo.Month.value << ", "
	"Year: " << m_TxPdo.Year.value << ", "
	"Roll_Offset: " << m_TxPdo.Roll_Offset.value << ", "
	"Pitch_Offset: " << m_TxPdo.Pitch_Offset.value << ", "
	"Yaw_Offset: " << m_TxPdo.Yaw_Offset.value << ", "
	"Grip_Offset: " << m_TxPdo.Grip_Offset.value << ", "
	"No_of_Usages: " << m_TxPdo.No_of_Usages.value << ", "
	"Max_Usages: " << m_TxPdo.Max_Usages.value << ", "
	"Digital_Inputs: " << m_TxPdo.Digital_Inputs.value << ", "
	"Grip_Counts: " << m_TxPdo.Grip_Counts.value << ", "
	"System_Number: " << m_TxPdo.System_Number.value << ", "
	"Device_UID: " << m_TxPdo.Device_UID.value << ", "
	"Spare_Bytes: " << m_TxPdo.Spare_Bytes.value << ", "
	"MFG_Day: " << m_TxPdo.MFG_Day.value << ", "
	"MFG_Month: " << m_TxPdo.MFG_Month.value << ", "
	"MFG_Year: " << m_TxPdo.MFG_Year.value
	<< std::endl;
}

void Ec_slave_rfidslave::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_SlaveAddr << " | " <<
	"SLAVE_NAME: " << m_SlaveName << " | " <<
	"Acknowledge: " << m_RxPdo.Acknowledge.value << ", "
	"Second: " << m_RxPdo.Second.value << ", "
	"Minute: " << m_RxPdo.Minute.value << ", "
	"Hour: " << m_RxPdo.Hour.value << ", "
	"Day: " << m_RxPdo.Day.value << ", "
	"Month: " << m_RxPdo.Month.value << ", "
	"Year: " << m_RxPdo.Year.value << ", "
	"No_of_Usages: " << m_RxPdo.No_of_Usages.value << ", "
	"Digital_Outputs: " << m_RxPdo.Digital_Outputs.value << ", "
	"Grip_Counts: " << m_RxPdo.Grip_Counts.value << ", "
	"System_Number: " << m_RxPdo.System_Number.value << ", "
	"Led_Red: " << m_RxPdo.Led_Red.value << ", "
	"Led_Green: " << m_RxPdo.Led_Green.value << ", "
	"LED_Blue: " << m_RxPdo.LED_Blue.value << ", "
	"Spare_Bytes: " << m_RxPdo.Spare_Bytes.value << ", "
	"Roll_Offset: " << m_RxPdo.Roll_Offset.value << ", "
	"Pitch_Offset: " << m_RxPdo.Pitch_Offset.value << ", "
	"Yaw_Offset: " << m_RxPdo.Yaw_Offset.value << ", "
	"Grip_Offset: " << m_RxPdo.Grip_Offset.value
	<< std::endl;
}
