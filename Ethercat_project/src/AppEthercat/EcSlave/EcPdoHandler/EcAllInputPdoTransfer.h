#ifndef EC_ALL_SLAVE_PDO_TRANSFER_H
#define EC_ALL_SLAVE_PDO_TRANSFER_H

#include "EcMaster.h"
#include "EcPdoVariable.h"
#include <iostream>

inline void transferAllInputPdoObject(EC_T_BYTE& txPdoData, EC_T_BYTE* pBuffer, const EC_T_CFG_SLAVE_INFO& slaveInfo)
{
    EC_GETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&txPdoData), slaveInfo.dwPdOffsIn, slaveInfo.dwPdSizeIn);
}

#endif // EC_ALL_SLAVE_PDO_TRANSFER_H
