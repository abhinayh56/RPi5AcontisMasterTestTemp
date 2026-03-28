#include "EcSlaveRfidslave.h"

EcSlaveRfidslave::EcSlaveRfidslave(uint16_t slaveAddr, const std::string &slaveName) : EcSlaveBase(slaveAddr, slaveName)
{
}

EcSlaveRfidslave::~EcSlaveRfidslave()
{
}

EC_T_DWORD EcSlaveRfidslave::registerTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Device_ID);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Second);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Minute);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Hour);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Day);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Month);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Year);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Roll_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Pitch_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Yaw_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Grip_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.No_of_Usages);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Max_Usages);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Grip_Counts);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.System_Number);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Device_UID);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Spare_Bytes);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.MFG_Day);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.MFG_Month);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.MFG_Year);

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::registerRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Acknowledge);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Second);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Minute);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Hour);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Day);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Month);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Year);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.No_of_Usages);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Digital_Outputs);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Grip_Counts);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.System_Number);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Led_Red);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Led_Green);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.LED_Blue);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Spare_Bytes);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Roll_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Pitch_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Yaw_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Grip_Offset);

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::transferTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Device_ID, pBuffer);
	transferInputPdoObject(m_txPdo.Second, pBuffer);
	transferInputPdoObject(m_txPdo.Minute, pBuffer);
	transferInputPdoObject(m_txPdo.Hour, pBuffer);
	transferInputPdoObject(m_txPdo.Day, pBuffer);
	transferInputPdoObject(m_txPdo.Month, pBuffer);
	transferInputPdoObject(m_txPdo.Year, pBuffer);
	transferInputPdoObject(m_txPdo.Roll_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.Pitch_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.Yaw_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.Grip_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.No_of_Usages, pBuffer);
	transferInputPdoObject(m_txPdo.Max_Usages, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_txPdo.Grip_Counts, pBuffer);
	transferInputPdoObject(m_txPdo.System_Number, pBuffer);
	transferInputPdoObject(m_txPdo.Device_UID, pBuffer);
	transferInputPdoObject(m_txPdo.Spare_Bytes, pBuffer);
	transferInputPdoObject(m_txPdo.MFG_Day, pBuffer);
	transferInputPdoObject(m_txPdo.MFG_Month, pBuffer);
	transferInputPdoObject(m_txPdo.MFG_Year, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::transferRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Acknowledge, pBuffer);
	transferOutputPdoObject(m_rxPdo.Second, pBuffer);
	transferOutputPdoObject(m_rxPdo.Minute, pBuffer);
	transferOutputPdoObject(m_rxPdo.Hour, pBuffer);
	transferOutputPdoObject(m_rxPdo.Day, pBuffer);
	transferOutputPdoObject(m_rxPdo.Month, pBuffer);
	transferOutputPdoObject(m_rxPdo.Year, pBuffer);
	transferOutputPdoObject(m_rxPdo.No_of_Usages, pBuffer);
	transferOutputPdoObject(m_rxPdo.Digital_Outputs, pBuffer);
	transferOutputPdoObject(m_rxPdo.Grip_Counts, pBuffer);
	transferOutputPdoObject(m_rxPdo.System_Number, pBuffer);
	transferOutputPdoObject(m_rxPdo.Led_Red, pBuffer);
	transferOutputPdoObject(m_rxPdo.Led_Green, pBuffer);
	transferOutputPdoObject(m_rxPdo.LED_Blue, pBuffer);
	transferOutputPdoObject(m_rxPdo.Spare_Bytes, pBuffer);
	transferOutputPdoObject(m_rxPdo.Roll_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Pitch_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Yaw_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Grip_Offset, pBuffer);

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::processTxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::processRxPdo()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::publishData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::subscribeData()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

EC_T_DWORD EcSlaveRfidslave::mainProcess()
{
	EC_T_DWORD dwRes = EC_E_NOERROR;

	return dwRes;
}

void EcSlaveRfidslave::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Device_ID: " << m_txPdo.Device_ID.value << ", "
	"Second: " << m_txPdo.Second.value << ", "
	"Minute: " << m_txPdo.Minute.value << ", "
	"Hour: " << m_txPdo.Hour.value << ", "
	"Day: " << m_txPdo.Day.value << ", "
	"Month: " << m_txPdo.Month.value << ", "
	"Year: " << m_txPdo.Year.value << ", "
	"Roll_Offset: " << m_txPdo.Roll_Offset.value << ", "
	"Pitch_Offset: " << m_txPdo.Pitch_Offset.value << ", "
	"Yaw_Offset: " << m_txPdo.Yaw_Offset.value << ", "
	"Grip_Offset: " << m_txPdo.Grip_Offset.value << ", "
	"No_of_Usages: " << m_txPdo.No_of_Usages.value << ", "
	"Max_Usages: " << m_txPdo.Max_Usages.value << ", "
	"Digital_Inputs: " << m_txPdo.Digital_Inputs.value << ", "
	"Grip_Counts: " << m_txPdo.Grip_Counts.value << ", "
	"System_Number: " << m_txPdo.System_Number.value << ", "
	"Device_UID: " << m_txPdo.Device_UID.value << ", "
	"Spare_Bytes: " << m_txPdo.Spare_Bytes.value << ", "
	"MFG_Day: " << m_txPdo.MFG_Day.value << ", "
	"MFG_Month: " << m_txPdo.MFG_Month.value << ", "
	"MFG_Year: " << m_txPdo.MFG_Year.value
	<< std::endl;
}

void EcSlaveRfidslave::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Acknowledge: " << m_rxPdo.Acknowledge.value << ", "
	"Second: " << m_rxPdo.Second.value << ", "
	"Minute: " << m_rxPdo.Minute.value << ", "
	"Hour: " << m_rxPdo.Hour.value << ", "
	"Day: " << m_rxPdo.Day.value << ", "
	"Month: " << m_rxPdo.Month.value << ", "
	"Year: " << m_rxPdo.Year.value << ", "
	"No_of_Usages: " << m_rxPdo.No_of_Usages.value << ", "
	"Digital_Outputs: " << m_rxPdo.Digital_Outputs.value << ", "
	"Grip_Counts: " << m_rxPdo.Grip_Counts.value << ", "
	"System_Number: " << m_rxPdo.System_Number.value << ", "
	"Led_Red: " << m_rxPdo.Led_Red.value << ", "
	"Led_Green: " << m_rxPdo.Led_Green.value << ", "
	"LED_Blue: " << m_rxPdo.LED_Blue.value << ", "
	"Spare_Bytes: " << m_rxPdo.Spare_Bytes.value << ", "
	"Roll_Offset: " << m_rxPdo.Roll_Offset.value << ", "
	"Pitch_Offset: " << m_rxPdo.Pitch_Offset.value << ", "
	"Yaw_Offset: " << m_rxPdo.Yaw_Offset.value << ", "
	"Grip_Offset: " << m_rxPdo.Grip_Offset.value
	<< std::endl;
}
