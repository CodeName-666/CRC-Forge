/**
 * @file Crc8Core.h
 * @brief Shared compile-time configured, non-reflected CRC8 calculation kernel.
 * @author Christof Seidel
 */
#ifndef CRC8_CORE_H
#define CRC8_CORE_H
#include "../Crc_Cfg.h"

/**
 * @brief Calculate a finalized CRC8 block without allocating storage.
 * @tparam Polynomial Non-reflected generator polynomial, excluding the top bit.
 * @tparam Initial Initial remainder for a first block.
 * @tparam XorOut Final XOR, undone before processing a continuation block.
 * @tparam TableSize Backend: 0, 16 or 256 entries; fixed at compile time.
 * @param[in] pData Readable bytes, or nullptr for an empty block.
 * @param[in] dataLength Number of bytes to process.
 * @param[in] seed Previous finalized checksum; only the low byte is used.
 * @param[in] firstCall True initializes a new message and ignores seed.
 * @param[in] pTable Polynomial-specific flash table; nullptr in CPU mode.
 * @return Finalized checksum, or zero for invalid pointers.
 * @pre A table backend receives exactly TableSize entries generated for Polynomial.
 * @note No mutable shared state. Platform table access is abstracted by M_CRC_READ8.
 */
template<uint8_t Polynomial, uint8_t Initial, uint8_t XorOut, uint16_t TableSize>
uint32_t calculateCrc8Block(const uint8_t* pData, uint32_t dataLength,
                          uint32_t seed, bool firstCall, const uint8_t* pTable) {
    static_assert(TableSize == 0U || TableSize == 16U || TableSize == 256U,
                  "Invalid CRC8 table size");
    uint32_t result = 0U;
    if (((pData != nullptr) || (dataLength == 0U))
        && ((TableSize == 0U) || (pTable != nullptr))) {
        uint8_t remainder = firstCall ? Initial : static_cast<uint8_t>(M_CRC_XOR(seed, XorOut));
        while (dataLength != 0U) {
            if (TableSize == 16U) {
                remainder = static_cast<uint8_t>(M_CRC_XOR(
                    M_CRC_READ8(pTable[M_CRC_XOR(M_CRC_SHIFT_RIGHT(remainder, 4U), M_CRC_SHIFT_RIGHT(*pData, 4U))]),
                    M_CRC_SHIFT_LEFT(remainder, 4U)));
                remainder = static_cast<uint8_t>(M_CRC_XOR(
                    M_CRC_READ8(pTable[M_CRC_XOR(M_CRC_SHIFT_RIGHT(remainder, 4U), M_CRC_AND(*pData, 0x0FU))]),
                    M_CRC_SHIFT_LEFT(remainder, 4U)));
            } else if (TableSize == 256U) {
                remainder = M_CRC_READ8(pTable[M_CRC_XOR(remainder, *pData)]);
            } else {
                remainder = static_cast<uint8_t>(M_CRC_XOR(remainder, *pData));
                for (uint8_t bit = 0U; bit < 8U; ++bit) {
                    const bool carry = M_CRC_AND(remainder, 0x80U) != 0U;
                    remainder = static_cast<uint8_t>(M_CRC_SHIFT_LEFT(remainder, 1U));
                    if (carry) {
                        remainder = static_cast<uint8_t>(M_CRC_XOR(remainder, Polynomial));
                    }
                }
            }
            ++pData;
            --dataLength;
        }
        result = static_cast<uint32_t>(M_CRC_XOR(remainder, XorOut));
    }
    return result;
}
#endif
