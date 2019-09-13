
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "crc16.h"

static uint16_t _table[256];

// ref. http://reveng.sourceforge.net/crc-catalogue/16.htm
const PARAM_CRC_T CRC16_PARAM_ARC = {
    .poly = 0x8005,
    .init = 0x0000,
    .xorOut = 0x0000,
    .check = 0xbb3d,
    .residue = 0x0000,
    .refIn = true,
    .refOut = true
};

const PARAM_CRC_T CRC16_PARAM_MODBUS = {
    .poly = 0x8005,
    .init = 0xFFFF,
    .xorOut = 0x0000,
    .check = 0x4b37,
    .residue = 0x0000,
    .refIn = true,
    .refOut = true
};

const PARAM_CRC_T CRC16_PARAM_KERMIT = {
    .poly = 0x1021,
    .init = 0x0000,
    .xorOut = 0x0000,
    .check = 0x2189,
    .residue = 0x0000,
    .refIn = true,
    .refOut = true
};

static PARAM_CRC_T _alg = {
    .poly = 0x8005,
    .init = 0x0000,
    .xorOut = 0x0000,
    .check = 0xbb3d,
    .residue = 0x0000,
    .refIn = true,
    .refOut = true
};

static const uint8_t rtab16[] = {
                    0,  8, 4, 12,
                    2, 10, 6, 14,
                    1,  9, 5, 13,
                    3, 11, 7, 15 };

static uint8_t reverseU8(const uint8_t data) {
    return (uint8_t)(
           (rtab16[data & 0xF] << 4) |
            rtab16[data >> 4 & 0xF]);
}

static uint16_t reverseU16(const uint16_t data) {
	return (uint16_t)(
		(rtab16[data & 0xF] << 12) |
		(rtab16[data >> 4 & 0xF] << 8) |
		(rtab16[data >> 8 & 0xF] << 4) |
		 rtab16[data >> 12 & 0xF]);
}

void makeCRCTable(const PARAM_CRC_T *params)
{
    uint16_t crc;

    if (params != NULL) {
        memcpy(&_alg, params, sizeof(_alg));
    }

    for (uint16_t n = 0; n < 256; n++) {
        crc = (uint16_t)(n << 8);
        for (uint16_t i = 0; i < 8; i++) {
            crc = (uint16_t) ((crc & 0x8000) ?
                (crc << 1) ^ _alg.poly :
                crc << 1);
        }
        _table[n] = crc;
    }
}

uint16_t calcCRC16(const uint8_t* pData, const size_t len)
{
    uint8_t tmp;
    uint16_t ret = _alg.init;

    for (unsigned i = 0; i < len; i++) {
        tmp = (_alg.refIn) ? reverseU8(pData[i]) : pData[i];
        ret = (uint16_t)(ret << 8 ^ _table[(ret >> 8) ^ tmp]);
    }
    return (_alg.refOut) ?
        reverseU16(ret) ^ _alg.xorOut :
        ret ^ _alg.xorOut;
}


#ifdef _MSC_VER
#if _MSC_VER > 1910 /* from VS2017, C99 */
int main()
{
    const uint8_t test[] = "123456789";
    #define numAlg  (3) // number of algorithm
    const PARAM_CRC_T* testParam[numAlg]= {
        &CRC16_PARAM_ARC,
        &CRC16_PARAM_MODBUS,
        &CRC16_PARAM_KERMIT
    };
    const uint8_t *strAlg[numAlg] = {
        "CRC16/ARC",
        "CRC16/MODBUS",
        "CRC16/KERMIT"
    };

    uint16_t result;
    uint16_t check;

    for (unsigned i = 0; i < numAlg; i++) {
        makeCRCTable(testParam[i]);
        result = calcCRC16(test, strlen(test));
        check = testParam[i]->check;
        printf("%s:0x%04x, expected:0x%04x\r\n", strAlg[i], result, check);
        printf("  calc result:%s\r\n", (result != check) ? "Failed" : "Passed");
    }

    return 0;
}
#endif
#endif
