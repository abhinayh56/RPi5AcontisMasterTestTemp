#ifndef EC_PDO_ELEMENT_H
#define EC_PDO_ELEMENT_H

#include "EcInterfaceCommon.h"

struct Ec_pdo_element
{
	EC_T_PROCESS_VAR_INFO pdo_info;
	bool isSupported = false;
};

#endif // EC_PDO_ELEMENT_H
