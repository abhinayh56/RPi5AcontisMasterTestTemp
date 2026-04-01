#ifndef EC_ALL_SLAVE_PDO_TRANSFER_H
#define EC_ALL_SLAVE_PDO_TRANSFER_H

#include "EcMaster.h"
#include "EcPdoVariable.h"
#include <iostream>

template<typename ObjectType>
void transferAllInputPdoObject(const EC_T_BYTE& txPdoData, EC_T_BYTE* pBuffer, const EC_T_CFG_SLAVE_INFO& slaveInfo)
{
    EC_GETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&txPdoData), slaveInfo.dwPdOffsIn, slaveInfo.nBitSizeIn);
}

void transferAllOutputPdoObject(const EC_T_BYTE& rxPdoData, EC_T_BYTE* pBuffer, const EC_T_CFG_SLAVE_INFO& slaveInfo)
{
    EC_SETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&rxPdoData), slaveInfo.dwPdOffsOut, slaveInfo.nBitSizeOut);
}

#endif // EC_ALL_SLAVE_PDO_TRANSFER_H
