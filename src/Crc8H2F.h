/**
 * @file Crc8H2F.h
 * @note Target: 8-bit AVR and 32-bit ESP32, restricted C++11, integer arithmetic only.
 * @note Not ISR-safe: call from a superloop or task; synchronize shared instances externally.
 * @brief CRC-8/AUTOSAR calculation with the shared nonvirtual lifecycle interface.
 * @author Christof Seidel
 */
#ifndef CRC8H2F_H
#define CRC8H2F_H

#include "../Crc_Cfg.h"
#include "CrcIf.h"

/** @brief Non-reflected generator polynomial used by this algorithm. */
#define CRC8H2F_POLYNOMIAL 0x2FU

/**
 * @brief Configurable CRC-8/AUTOSAR calculator with direct static computation.
 * @details Polynomial 0x2F, initial value 0xFF, final XOR 0xFF.
 * The check value for "123456789" is 0xDF.
 * CRC8H2F_TABLE_SIZE selects CPU, 16-entry or 256-entry calculation at compile time.
 * Inherits buffer configuration, status, start(), loop(), get(), and cancel()
 * from CrcIf<Crc8H2F>. All calls are nonvirtual and require no dynamic storage.
 * Static calculations operate without an instance and do not change progress.
 * @note The input buffer is borrowed. Keep it valid and unchanged while processing.
 * @see CrcIf for detailed lifecycle contracts and thread-safety requirements.
 */
class Crc8H2F : public CrcIf<Crc8H2F> {
public:
    /**
     * @brief Construct an idle calculator with optional borrowed input.
     * @param[in] pData Input buffer; may be null before configuration.
     * @param[in] dataLen Number of readable bytes; cooperative start() rejects zero.
     */
    Crc8H2F(uint8_t* pData = 0, uint32_t dataLen = 0) : CrcIf<Crc8H2F>(pData, dataLen) {}

    /**
     * @brief Calculate the configured buffer synchronously as a complete message.
     * @return Finalized CRC; cooperative progress remains unchanged.
     * @see CrcIf::calculate() for empty-buffer and invalid-input behavior.
     */
    uint32_t calculate() const { return CrcIf<Crc8H2F>::calculate(); }

    /**
     * @brief Calculate or continue a CRC-8/AUTOSAR (H2F) checksum.
     * @param[in] pData Buffer containing at least dataLength readable bytes.
     * May be null for an empty block. Input bytes are never modified.
     * @param[in] dataLength Number of bytes to process.
     * @param[in] startValue Previously returned finalized CRC; only its low 8
     * bits are used. Ignored when isFirstCall is true.
     * @param[in] isFirstCall True uses the fixed initial value 0xFF; false restores
     * the internal remainder from the supplied previous checksum.
     * @return Finalized 8-bit CRC, zero-extended to uint32_t; returns zero
     * for a null pointer with nonzero dataLength.
     * @details Empty first blocks return zero. Empty continuation blocks
     * return the supplied checksum masked to the algorithm width.
     * @note Argument order differs from the Crc static wrappers: startValue precedes
     * isFirstCall here. Zero is also a valid checksum, not a unique error code.
     */
    static uint32_t calculate(const uint8_t* pData, uint32_t dataLength, uint32_t startValue = CRC8_INITIAL_VALUE, bool isFirstCall = true);

private:
    friend class CrcIf<Crc8H2F>;
    /** @brief Report compile-time availability. @return True when this algorithm is enabled. */
    bool isAlgorithmEnabled() const { return CRC8H2F_ENABLED == CRC_ENABLED; }
    /**
     * @brief Bind the shared lifecycle to this concrete static calculation.
     * @param[in] pData Readable input buffer.
     * @param[in] dataLen Number of input bytes.
     * @param[in] startValue Previous finalized CRC, ignored for a first block.
     * @param[in] firstCall True initializes a complete message; false continues it.
     * @return Finalized checksum for the supplied block.
     */
    uint32_t calculateBlock(const uint8_t* pData, uint32_t dataLen,
                            uint32_t startValue, bool firstCall) const {
        return Crc8H2F::calculate(pData, dataLen, startValue, firstCall);
    }
};
#endif
