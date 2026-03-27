#ifndef EC_PDO_ELEMENT_H
#define EC_PDO_ELEMENT_H

#include "EcMaster.h"
#include <iostream>

template<EC_T_WORD ObjectIndex, EC_T_BYTE ObjectSubIndex, typename ValueType>
struct PdoVariable
{
    typedef ValueType value_type;

    static const EC_T_WORD objectIndex = ObjectIndex;
    static const EC_T_BYTE objectSubIndex = ObjectSubIndex;
    std::string                    objectName;
    EC_T_PROCESS_VAR_INFO       variableInfo;
    ValueType                   value;
    bool                   isSupported;
};

inline uint32_t lookupInputObjectInfoByIndex(const uint16_t slaveAddress, const uint16_t objectIndex, const uint16_t objectSubIndex, EC_T_PROCESS_VAR_INFO& objectInfo, bool& isSupported)
{
    EC_T_DWORD slaveId = ecatGetSlaveId(slaveAddress);

    isSupported = false;

    std::cout << "-----------\n";
    std::cout << "Slave ID = " << slaveId << std::endl;
    std::cout << "Slave address = " << slaveAddress << std::endl;
    std::cout << "Object Index = " << std::hex << objectIndex << std::endl;
    std::cout << "Object Sub Index = " << std::hex << objectSubIndex << std::endl;

    EC_T_WORD numOfVarsToRead;

    EC_T_WORD wReadEntries;

    EC_T_DWORD result = ecatGetSlaveInpVarInfoNumOf(EC_TRUE, slaveAddress, &numOfVarsToRead);

    if (result != EC_E_NOERROR)
    {
        std::cout << "Slave not found " << std::endl;
        return 1<<10;
    }

    std::vector<EC_T_PROCESS_VAR_INFO_EX> variables(numOfVarsToRead);

    result = ecatGetSlaveInpVarInfoEx(EC_TRUE, slaveAddress, numOfVarsToRead, variables.data(), &wReadEntries);

    if (result != EC_E_NOERROR)
    {
        std::cout << "ecatGetSlaveInpVarInfoEx failed: entries read = " << std::dec << wReadEntries << std::endl;
        return 1<<10;
    }

    if (variables.empty() || wReadEntries == 0)
    {
        std::cout << "ecatGetSlaveInpVarInfoEx failed: variables are empty " << std::dec << wReadEntries << std::endl;
        return 1<<10;
    }

    for (uint32_t ii = 0; ii < wReadEntries; ++ii)
    {
        if ((variables[ii].wIndex == objectIndex) && (variables[ii].wSubIndex == objectSubIndex))
        {
            std::cout << "\t Match found " << variables[ii].szName << std::endl;
            ecatFindInpVarByName(variables[ii].szName, &objectInfo);
            isSupported = true;
            break;
        }
    }

    std::cout << "Input PDO Object Index = " << std::hex << objectIndex << std::dec << (isSupported ? "FOUND" : "NOT FOUND") << std::endl;

    return isSupported ? 0 : 1<<10;
}

template<typename ObjectType>
uint32_t lookupInputPdoObject(const uint16_t slaveAddress, ObjectType& object)
{
    uint32_t retVal = lookupInputObjectInfoByIndex(slaveAddress, object.objectIndex, object.objectSubIndex, object.variableInfo, object.isSupported);

    if (retVal != 0)
    {
        std::cout << "lookupInputPdoObject - FAILED : slave = " << slaveAddress << "\n";
        std::cout << "\tobject index = " << std::hex << std::showbase << object.objectIndex << "\n";
        std::cout << "\tobject subindex = " << std::hex << std::showbase << static_cast<uint16_t>(object.objectSubIndex) << std::endl;
        return retVal;
    }

    if (static_cast<EC_T_INT>(sizeof(object.value)) * 8 >= object.variableInfo.nBitSize)
    {
        std::cout << "\tlookupInputPdoObject succeeded for " << object.variableInfo.szName << "\n";
        return retVal;
    }

    object.isSupported = false;
    std::cout << "\t size mismatch " << "\tobject name = " << object.variableInfo.szName << "\n" << "\t value size: " << sizeof(object.value) * 8 << ", nBitSize: " << object.variableInfo.nBitSize << std::endl;
    return 1<<10;
}

template<typename ObjectType>
void transferInputPdoObject(ObjectType& object, EC_T_BYTE* pBuffer)
{
    if (object.isSupported)
    {
        EC_GETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&object.value), object.variableInfo.nBitOffs, object.variableInfo.nBitSize);
    }
}

inline uint32_t lookupOutputObjectInfoByIndex(const uint16_t slaveAddress, const uint16_t objectIndex, const uint16_t  objectSubIndex, EC_T_PROCESS_VAR_INFO& objectInfo, bool& isSupported)
{
    EC_T_DWORD slaveId = ecatGetSlaveId(slaveAddress);

    isSupported = false;

    std::cout << "-----------\n";
    std::cout << "Slave ID = " << slaveId << std::endl;
    std::cout << "Slave address = " << slaveAddress << std::endl;
    std::cout << "Object Index = " << std::hex << objectIndex << std::endl;
    std::cout << "Object Sub Index = " << std::hex << objectSubIndex << std::endl;

    EC_T_WORD numOfVarsToRead;

    EC_T_WORD wReadEntries;

    EC_T_DWORD result = ecatGetSlaveOutpVarInfoNumOf(EC_TRUE, slaveAddress, &numOfVarsToRead);

    if (result != EC_E_NOERROR)
    {
        std::cout << "Slave not found " << std::endl;
        return 1<<10;
    }

    std::vector<EC_T_PROCESS_VAR_INFO_EX> variables(numOfVarsToRead);

    result = ecatGetSlaveOutpVarInfoEx(EC_TRUE, slaveAddress, numOfVarsToRead, variables.data(), &wReadEntries);

    if (result != EC_E_NOERROR)
    {
        std::cout << "ecatGetSlaveOutpVarInfoEx failed: entries read = " << std::dec << wReadEntries << std::endl;
        return 1<<10;
    }

    if (variables.empty() || wReadEntries == 0)
    {
        std::cout << "ecatGetSlaveOutpVarInfoEx failed: variables are empty " << std::dec << wReadEntries << std::endl;
        return 1<<10;
    }

    for (uint32_t ii = 0; ii < wReadEntries; ++ii)
    {
        if ((variables[ii].wIndex == objectIndex) && (variables[ii].wSubIndex == objectSubIndex))
        {
            std::cout << "\t Match found " << variables[ii].szName << std::endl;
            ecatFindOutpVarByName(variables[ii].szName, &objectInfo);
            isSupported = true;
            break;
        }
    }

    std::cout << "Output PDO Object Index = " << std::hex << objectIndex << std::dec << (isSupported ? " FOUND" : " NOT FOUND") << std::endl;

    return isSupported ? 0 : 1<<10;
}

template<typename ObjectType>
uint32_t lookupOutputPdoObject(const uint16_t slaveAddress, ObjectType& object)
{
    uint32_t retVal = lookupOutputObjectInfoByIndex(slaveAddress, object.objectIndex, object.objectSubIndex, object.variableInfo, object.isSupported);

    if (retVal != 0)
    {
        std::cout << "\tlookupInputPdoObject - FAILED : slave = " << slaveAddress << "\n";
        std::cout << "\tobject index = " << std::hex << std::showbase << object.objectIndex << "\n";
        std::cout << "\tobject subindex = " << std::hex << std::showbase << static_cast<uint16_t>(object.objectSubIndex) << std::endl;
        return retVal;
    }

    if (static_cast<EC_T_INT>(sizeof(object.value)) * 8 <= object.variableInfo.nBitSize)
    {
        std::cout << "\tlookupOutputPdoObject succeeded for " << object.variableInfo.szName << "\n";
        return retVal;
    }

    object.isSupported = false;
    std::cout << "\t size mismatch " << "\tobject name = " << object.variableInfo.szName << "\n" << "\t value size: " << sizeof(object.value) * 8 << ", nBitSize: " << object.variableInfo.nBitSize << std::endl;
    return 1<<10;
}

template<typename ObjectType>
void transferOutputPdoObject(ObjectType& object, EC_T_BYTE* pBuffer)
{
    if (object.isSupported)
    {
        EC_SETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&object.value), object.variableInfo.nBitOffs, object.variableInfo.nBitSize);
    }
}

//struct TxPdo
//{
//    PdoVariable<INDEX_TXPDO_STATUS_WORD, uint16_t>                  statusWord;
//    PdoVariable<INDEX_TXPDO_MODE_OF_OPERATION_DISPLAY, uint8_t>     modesOfOperationDisplay;
//    PdoVariable<INDEX_TXPDO_POSITION_ACTUAL_VALUE, int32_t>      positionActual;
//    PdoVariable<INDEX_TXPDO_VELOCITY_ACTUAL_VALUE, int32_t>      velocityActual;
//    PdoVariable<INDEX_TXPDO_TORQUE_ACTUAL_VALUE, int16_t>        torqueActual;
//    PdoVariable<INDEX_TXPDO_DIGITAL_INPUTS, uint32_t>               digitalInputs;
//    PdoVariable<INDEX_TXPDO_LAST_ERROR, int32_t>			     lastError;
//} m_TxPdo;
//lookupInputPdoObject(m_TxPdo.statusWord);
//EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();
//transferInputPdoObject(m_TxPdo.statusWord, pBuffer);
//
//struct RxPdo
//{
//    PdoVariable<INDEX_RXPDO_CONTROL_WORD, uint16_t>                 controlWord;
//    PdoVariable<INDEX_RXPDO_MODE_OF_OPERATION, uint8_t>             modesOfOperation;
//    PdoVariable<INDEX_RXPDO_TARGET_POSITION, int32_t>            targetPosition;
//    PdoVariable<INDEX_RXPDO_TARGET_TORQUE, int16_t>              targetTorque;
//    PdoVariable<INDEX_RXPDO_INTERPOLATED_DATA_RECORD, int32_t>   interpolatedDataRecord;
//    PdoVariable<INDEX_RXPDO_DIGITAL_OUTPUTS, uint32_t>              digitalOutputs;
//    PdoVariable<INDEX_RXPDO_ANALOG_OUTPUT_1_VALUE, uint32_t>        analogValue1;
//} m_RxPdo;
//lookupOutputPdoObject(m_RxPdo.controlWord);
//EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();
//transferOutputPdoObject(m_RxPdo.controlWord, pBuffer);

#endif // EC_PDO_ELEMENT_H
