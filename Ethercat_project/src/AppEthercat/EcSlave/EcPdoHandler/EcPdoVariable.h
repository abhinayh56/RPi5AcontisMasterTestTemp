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

#endif // EC_PDO_VARIABLE_H
