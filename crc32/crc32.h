
#pragma once

#ifndef _CRC32_H_
#define _CRC32_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct _crc32_param_t {
    uint32_t poly;  // Polynomial function
    uint32_t init;  // initial val
    uint32_t xorOut;
    uint32_t check; // for "123456789" result
    uint32_t residue;
    bool     refIn;
    bool     refOut;
} PARAM_CRC_T;

// predefinition algorithm
extern const PARAM_CRC_T   CRC32_PARAM_BZIP2;
extern const PARAM_CRC_T   CRC32_PARAM_CDROM;
extern const PARAM_CRC_T   CRC32_PARAM_ISOHDLC;
// export function
extern void makeCRCTable(const PARAM_CRC_T *params);
extern uint32_t calcCRC32(const uint8_t* pData, const size_t len);

#endif /* end of _CRC16_H_ */
