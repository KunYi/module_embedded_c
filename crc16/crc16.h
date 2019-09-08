
#pragma once

#ifndef _CRC16_H_
#define _CRC16_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct _crc16_param_t {
    uint16_t poly;  // Polynomial function
    uint16_t init;  // initial val
    uint16_t xorOut;
    uint16_t check; // for "123456789" result
    uint16_t residue;
    bool     refIn;
    bool     refOut;
} PARAM_CRC_T;

// predefinition algorithm
extern const PARAM_CRC_T 	CRC16_PARAM_ARC;
extern const PARAM_CRC_T 	CRC16_PARAM_MODBUS;
extern const PARAM_CRC_T 	CRC16_PARAM_KERMIT;

// export function
extern void makeCRCTable(const PARAM_CRC_T *params);
extern uint16_t calcCRC16(const uint8_t* pData, const size_t len);

#endif /* end of _CRC16_H_ */