#ifndef EC_PDO_VARIABLE_H
#define EC_PDO_VARIABLE_H

#include "EcMaster.h"
#include <iostream>

template<EC_T_WORD ObjectIndex, EC_T_BYTE ObjectSubIndex, typename ValueType> // can include value size in template parameter
struct PdoVariable
{
    typedef ValueType value_type;

    static const EC_T_WORD objectIndex = ObjectIndex;
    static const EC_T_BYTE objectSubIndex = ObjectSubIndex;
    std::string objectName;
    EC_T_PROCESS_VAR_INFO variableInfo;
    ValueType value;
    bool isSupported;
};

//struct TxPdo
//{
//    PdoVariable<INDEX_TXPDO_STATUS_WORD, uint16_t>               statusWord;
//    PdoVariable<INDEX_TXPDO_MODE_OF_OPERATION_DISPLAY, uint8_t>  modesOfOperationDisplay;
//    PdoVariable<INDEX_TXPDO_POSITION_ACTUAL_VALUE, int32_t>      positionActual;
//    PdoVariable<INDEX_TXPDO_VELOCITY_ACTUAL_VALUE, int32_t>      velocityActual;
//    PdoVariable<INDEX_TXPDO_TORQUE_ACTUAL_VALUE, int16_t>        torqueActual;
//    PdoVariable<INDEX_TXPDO_DIGITAL_INPUTS, uint32_t>            digitalInputs;
//    PdoVariable<INDEX_TXPDO_LAST_ERROR, int32_t>                 lastError;
//} m_TxPdo;

//lookupInputPdoObject(m_TxPdo.statusWord);
//lookupInputPdoObject(m_TxPdo.modesOfOperationDisplay);
//lookupInputPdoObject(m_TxPdo.positionActual);
//lookupInputPdoObject(m_TxPdo.velocityActual);
//lookupInputPdoObject(m_TxPdo.torqueActual);
//lookupInputPdoObject(m_TxPdo.digitalInputs);
//lookupInputPdoObject(m_TxPdo.lastError);

//EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();
//transferInputPdoObject(m_TxPdo.statusWord, pBuffer);
//transferInputPdoObject(m_TxPdo.modesOfOperationDisplay, pBuffer);
//transferInputPdoObject(m_TxPdo.positionActual, pBuffer);
//transferInputPdoObject(m_TxPdo.velocityActual, pBuffer);
//transferInputPdoObject(m_TxPdo.torqueActual, pBuffer);
//transferInputPdoObject(m_TxPdo.digitalInputs, pBuffer);
//transferInputPdoObject(m_TxPdo.lastError, pBuffer);

//struct RxPdo
//{
//    PdoVariable<INDEX_RXPDO_CONTROL_WORD, uint16_t>            controlWord;
//    PdoVariable<INDEX_RXPDO_MODE_OF_OPERATION, uint8_t>        modesOfOperation;
//    PdoVariable<INDEX_RXPDO_TARGET_POSITION, int32_t>          targetPosition;
//    PdoVariable<INDEX_RXPDO_TARGET_TORQUE, int16_t>            targetTorque;
//    PdoVariable<INDEX_RXPDO_INTERPOLATED_DATA_RECORD, int32_t> interpolatedDataRecord;
//    PdoVariable<INDEX_RXPDO_DIGITAL_OUTPUTS, uint32_t>         digitalOutputs;
//    PdoVariable<INDEX_RXPDO_ANALOG_OUTPUT_1_VALUE, uint32_t>   analogValue1;
//} m_RxPdo;

//lookupOutputPdoObject(m_RxPdo.controlWord);
//lookupOutputPdoObject(m_RxPdo.modesOfOperation);
//lookupOutputPdoObject(m_RxPdo.targetPosition);
//lookupOutputPdoObject(m_RxPdo.targetTorque);
//lookupOutputPdoObject(m_RxPdo.interpolatedDataRecord);
//lookupOutputPdoObject(m_RxPdo.digitalOutputs);
//lookupOutputPdoObject(m_RxPdo.analogValue1);

//EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();
//transferOutputPdoObject(m_RxPdo.controlWord, pBuffer);
//transferOutputPdoObject(m_RxPdo.modesOfOperation, pBuffer);
//transferOutputPdoObject(m_RxPdo.targetPosition, pBuffer);
//transferOutputPdoObject(m_RxPdo.targetTorque, pBuffer);
//transferOutputPdoObject(m_RxPdo.interpolatedDataRecord, pBuffer);
//transferOutputPdoObject(m_RxPdo.digitalOutputs, pBuffer);
//transferOutputPdoObject(m_RxPdo.analogValue1, pBuffer);


//void transferOutputPdoSlave(uint8_t& value, EC_T_BYTE* pBuffer, EC_T_CFG_SLAVE_INFO& slaveInfo)
//{
//	EC_SETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&value), slaveInfo.dwPdOffsOut, slaveInfo.dwPdSizeOut);
//}


#endif // EC_PDO_VARIABLE_H
