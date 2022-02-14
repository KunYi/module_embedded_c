
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "crc32.h"

static uint32_t _table[256];

// ref. https://reveng.sourceforge.io/crc-catalogue/17plus.htm
const PARAM_CRC_T CRC32_PARAM_BZIP2 = {
    .poly = 0x04c11db7,
    .init = 0xffffffff,
    .xorOut = 0xffffffff,
    .check = 0xfc891918,
    .residue = 0xc704dd7b,
    .refIn = false,
    .refOut = false
};

const PARAM_CRC_T CRC32_PARAM_CDROM = {
    .poly = 0x8001801b,
    .init = 0x00000000,
    .xorOut = 0x00000000,
    .check = 0x6ec2edc4,
    .residue = 0x00000000,
    .refIn = true,
    .refOut = true
};

const PARAM_CRC_T CRC32_PARAM_ISOHDLC = {
    .poly = 0x04c11db7,
    .init = 0xffffffff,
    .xorOut = 0xffffffff,
    .check = 0xcbf43926,
    .residue = 0xdebb20e3,
    .refIn = true,
    .refOut = true
};

static PARAM_CRC_T _alg = {
    .poly = 0x04c11db7,
    .init = 0xffffffff,
    .xorOut = 0xffffffff,
    .check = 0xcbf43926,
    .residue = 0xdebb20e3,
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

static uint32_t reverseU32(const uint32_t data) {
    return (uint32_t) (
        (rtab16[data & 0xF] << 28) |
        (rtab16[data >> 4 & 0xF] << 24) |
        (rtab16[data >> 8 & 0xF] << 20) |
        (rtab16[data >> 12 & 0xF] << 16) |
        (rtab16[data >> 16 & 0xF] << 12) |
        (rtab16[data >> 20 & 0xF] << 8) |
        (rtab16[data >> 24 & 0xF] << 4) |
        (rtab16[data >> 28 & 0xF]));
}

void makeCRCTable(const PARAM_CRC_T *params)
{
    uint32_t crc;

    if (params != NULL) {
        memcpy(&_alg, params, sizeof(_alg));
    }

    for (uint32_t n = 0; n < 256; n++) {
        crc = (uint32_t)(n << 24);
        for (uint32_t i = 0; i < 8; i++) {
            crc = (uint32_t) ((crc & 0x80000000) ?
                ((crc << 1) ^ _alg.poly) :
                (crc << 1));
        }
        _table[n] = crc;
    }
}

uint32_t calcCRC32(const uint8_t* pData, const size_t len)
{
    uint8_t tmp;
    uint32_t ret = _alg.init;

    for (unsigned i = 0; i < len; i++) {
        tmp = (_alg.refIn) ? reverseU8(pData[i]) : pData[i];
        ret = (uint32_t)((ret << 8) ^ _table[(ret >> 24) ^ tmp]);
    }
    return (_alg.refOut) ?
        reverseU32(ret) ^ _alg.xorOut :
        ret ^ _alg.xorOut;
}


#ifdef _MSC_VER
#if _MSC_VER > 1910 /* from VS2017, C99 */
int main()
{
    const uint8_t test[] = "123456789";
    #define numAlg  (3) // number of algorithm
    const PARAM_CRC_T* testParam[numAlg]= {
        &CRC32_PARAM_BZIP2,
        &CRC32_PARAM_CDROM,
        &CRC32_PARAM_ISOHDLC
    };
    const uint8_t *strAlg[numAlg] = {
        "CRC-32/BZIP2",
        "CRC-32/CD-ROM-EDC",
        "CRC-32/ISO-HDLC",
    };

    uint32_t result;
    uint32_t check;

    for (unsigned i = 0; i < numAlg; i++) {
        makeCRCTable(testParam[i]);
        result = calcCRC32(test, strlen(test));
        check = testParam[i]->check;
        printf("%s:0x%04x, expected:0x%04x\r\n", strAlg[i], result, check);
        printf("  calc result:%s\r\n", (result != check) ? "Failed" : "Passed");
    }

    return 0;
}
#endif
#endif
