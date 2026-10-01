/**
 * @file Crc16.cpp
 * @brief CRC-16/IBM-3740 bitwise and lookup-table implementation.
 * @author Christof Seidel
 */

#include "Crc16.h"
#include <stddef.h>

#if CRC16_ENABLED == CRC_ENABLED




#if (CRC16_TABLE_SIZE > 0U) /* CRC16 generation via table */

/* Table of pre-computed values for CRC16. Used Polynomial is 0x1021 */
extern const uint16_t Crc16_Table[CRC16_TABLE_SIZE] CRC_TABLE_STORAGE;
#endif

/**
 * @details CRC16_TABLE_SIZE selects the backend at compile time.
 * The 16-entry backend performs two nibble lookups per byte; the 256-entry
 * backend performs one byte lookup. CPU mode performs eight polynomial steps
 * per byte and stores no lookup table. AVR reads use program-memory macros.
 */
uint32_t Crc16::calculate(const uint8_t* pData, uint32_t dataLength,
                            uint32_t seed, bool isFirstCall)
{
    uint32_t result = 0U;
    if ((pData != nullptr) || (dataLength == 0U)) {
        uint16_t remainder = isFirstCall ? static_cast<uint16_t>(CRC16_INITIAL_VALUE)
            : static_cast<uint16_t>(M_CRC_XOR(seed, 0U));
        while (dataLength != 0U) {
#if CRC16_TABLE_SIZE == CRC_SMALL_TABLE_CALCULATION
            remainder = static_cast<uint16_t>(M_CRC_XOR(
                M_CRC_READ16(Crc16_Table[M_CRC_XOR(M_CRC_SHIFT_RIGHT(remainder, 12U), M_CRC_SHIFT_RIGHT(*pData, 4U))]),
                M_CRC_SHIFT_LEFT(remainder, 4U)));
            remainder = static_cast<uint16_t>(M_CRC_XOR(
                M_CRC_READ16(Crc16_Table[M_CRC_XOR(M_CRC_SHIFT_RIGHT(remainder, 12U), M_CRC_AND(*pData, 0x0FU))]),
                M_CRC_SHIFT_LEFT(remainder, 4U)));
#elif CRC16_TABLE_SIZE == CRC_LARGE_TABLE_CALCULATION
            remainder = static_cast<uint16_t>(M_CRC_XOR(
                M_CRC_READ16(Crc16_Table[M_CRC_XOR(M_CRC_SHIFT_RIGHT(remainder, 8U), *pData)]),
                M_CRC_SHIFT_LEFT(remainder, 8U)));
#else
            remainder = static_cast<uint16_t>(M_CRC_XOR(remainder, M_CRC_SHIFT_LEFT(*pData, 8U)));
            for (uint8_t bit = 0U; bit < 8U; ++bit) {
                const bool carry = M_CRC_AND(remainder, 0x8000U) != 0U;
                remainder = static_cast<uint16_t>(M_CRC_SHIFT_LEFT(remainder, 1U));
                if (carry) {
                    remainder = static_cast<uint16_t>(M_CRC_XOR(remainder, CRC16_POLYNOMIAL));
                }
            }
#endif
            ++pData;
            --dataLength;
        }
        result = static_cast<uint32_t>(M_CRC_XOR(remainder, 0U));
    }
    return result;
}

#if (CRC16_TABLE_SIZE != CRC_SYSTEM_CALCULATION) /* CRC16 generation via table */

/**
 * @brief Precomputed polynomial remainders for the selected table backend.
 * @details Small tables advance four bits per lookup; large tables advance
 * eight. The array is omitted entirely in CPU mode. CRC_TABLE_STORAGE places
 * the constant entries in program memory on AVR to preserve RAM.
 */
const uint16_t Crc16_Table[CRC16_TABLE_SIZE] CRC_TABLE_STORAGE =
{
   0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
   0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
#if (CRC16_TABLE_SIZE == CRC_LARGE_TABLE_CALCULATION)
   0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
   0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
   0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
   0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
   0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
   0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
   0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
   0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
   0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
   0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
   0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
   0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
   0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
   0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
   0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
   0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
   0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
   0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
   0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
   0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
   0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
   0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
   0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
   0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
   0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
   0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
   0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
   0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
   0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
   0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U
#endif
};

#endif



#endif // CRC16_ENABLED
