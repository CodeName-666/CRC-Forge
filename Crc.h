/**
 * @file Crc.h
 * @note Target: 8-bit AVR and 32-bit ESP32, restricted C++11, integer arithmetic only.
 * @note Not ISR-safe: call from a superloop or task; synchronize shared instances externally.
 * @brief Bridge to the selected CRC algorithm and its configuration.
 * @author Christof Seidel
 */
#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include "Crc_Cfg.h"
#include "Crc_Types.h"
#include "src/CrcIf.h"
#if CRC8_ENABLED == CRC_ENABLED
#include "src/Crc8.h"
#endif
#if CRC8H2F_ENABLED == CRC_ENABLED
#include "src/Crc8H2F.h"
#endif
#if CRC16_ENABLED == CRC_ENABLED
#include "src/Crc16.h"
#endif
#if CRC32_ENABLED == CRC_ENABLED
#include "src/Crc32.h"
#endif


/**
 * @class Crc
 * @brief Select a CRC algorithm at runtime using Algorithm_E.
 * @details Supports CRC-8/SAE-J1850, CRC-8/AUTOSAR (H2F),
 * CRC-16/IBM-3740 (CCITT-FALSE), and CRC-32/ISO-HDLC.
 * Static methods calculate a complete buffer or continue a previous result.
 * CrcIf<Crc> owns the sole configuration and cooperative state. The bridge
 * selects the calculation using the enum and directly calls algorithm functions.
 * No algorithm objects, virtual calls, function pointers or manual lifetimes are needed.
 *
 * The shared interface implements the cooperative lifecycle: CRC_NO_CALC -> CRC_CALC_ACTIVE ->
 * CRC_CALC_FINISHED -> CRC_NO_CALC. start(), loop(), and get() perform
 * these transitions. cancel() stops active work; every setter discards work
 * and unread results, even if the configured value does not change.
 *
 * Input buffers are borrowed, never copied or freed. Keep them valid and
 * unchanged until synchronous processing returns or cooperative work ends.
 * @note Concurrent access to the same instance requires external synchronization.
 * Static calls and separate instances do not share mutable calculation state.
 * @see Crc_Cfg.h for compile-time algorithm and backend selection.
 */
class Crc : public CrcIf<Crc> {
public:
    /**
     * @brief Construct an idle CRC calculator.
     * @details Selects CRC_8 with a null data pointer, zero length and zero result.
     * CRC_8 remains the default even when that algorithm is disabled.
     */
    Crc();
    /**
     * @brief Destroy the calculator without freeing the caller-owned input buffer.
     */
    ~Crc();
    /**
     * @brief Read the configured buffer length.
     * @return Number of bytes configured by setDataLen(), not the remaining count.
     */
    uint32_t getDataLen() const;
    /**
     * @brief Read the borrowed input pointer.
     * @return Configured buffer address, or a null pointer when no buffer is set.
     * @warning Do not modify the buffer while a calculation is active.
     */
    uint8_t* getDataPtr() const;
    /**
     * @brief Read the selected algorithm.
     * @return Stored algorithm identifier; this does not validate its availability.
     */
    Algorithm_E getType() const;

    /**
     * @brief Configure the number of bytes and discard any previous work.
     * @param[in] dataLen Buffer length in bytes. Zero is valid for synchronous calls
     * but start() rejects it for cooperative processing.
     * @post State is CRC_NO_CALC; the result and processed-byte count are zero.
     */
    void setDataLen(uint32_t dataLen);
    /**
     * @brief Configure a borrowed input buffer and discard any previous work.
     * @param[in] pData Address of at least getDataLen() readable bytes, or null.
     * The library never modifies, copies, owns or frees these bytes.
     * @post State is CRC_NO_CALC; the result and processed-byte count are zero.
     */
    void setDataPtr(uint8_t* pData);
    /**
     * @brief Select an algorithm and discard any previous work.
     * @param[in] type Algorithm identifier. Availability is checked when calculating
     * or starting; this setter also stores unknown or disabled identifiers.
     * @post State is CRC_NO_CALC; the result and processed-byte count are zero.
     */
    void setType(Algorithm_E type);


    /**
     * @brief Calculate the configured buffer synchronously as a new message.
     * @return Finalized CRC, or zero for an invalid buffer or unavailable algorithm.
     * @details Uses the configured type, pointer and length with firstCall=true.
     * An empty buffer is supported. The cooperative state, byte count and pending
     * result are unchanged; the return value is not subsequently available via get().
     * @see calculate(Algorithm_E, const uint8_t*, uint32_t, bool, uint32_t)
     */
    uint32_t calculate();

    /**
     * @brief Begin cooperative processing without consuming input bytes yet.
     * @retval true A nonempty, nonnull buffer and enabled algorithm were accepted.
     * @retval false The configuration is invalid or the state is not CRC_NO_CALC.
     * @post On success, clears the previous result and byte count and sets
     * CRC_CALC_ACTIVE. On failure, leaves the current state unchanged.
     * @note Consume a finished result with get(), or use a setter to discard it,
     * before starting another calculation.
     */
    bool start();
    /**
     * @brief Abort an active cooperative calculation.
     * @retval true Active work was discarded and the state is now CRC_NO_CALC.
     * @retval false No active calculation existed; no state was changed.
     * @details Clears the result and byte count while preserving the configuration.
     * A finished but unread result is not discarded by this method.
     */
    bool cancel();
    /**
     * @brief Compatibility alias for cancel(), retaining the original spelling.
     * @return The result of cancel().
     * @see cancel()
     */
    bool cancle();
    /**
     * @brief Check whether a cooperative result is ready.
     * @return True only in CRC_CALC_FINISHED; checking does not consume the result.
     */
    bool isFinished();
    /**
     * @brief Consume a completed cooperative result.
     * @return Finalized CRC when finished, otherwise zero.
     * @post A finished result is cleared and the state becomes CRC_NO_CALC.
     * If no result is ready, the state and partial calculation are unchanged.
     * @note Zero is also a valid checksum. Use isFinished() to determine readiness.
     */
    uint32_t get();
    /**
     * @brief Process exactly one byte of an active cooperative calculation.
     * @details Does nothing unless the state is CRC_CALC_ACTIVE. Delegates
     * state transitions to CrcIf and byte calculation to the selected static function.
     * The call processing the last byte immediately sets CRC_CALC_FINISHED.
     * @pre After a successful start(), the configured buffer must remain valid and
     * unchanged until completion or cancellation. There is no internal locking.
     */
    void loop();
    /**
     * @brief Inspect the cooperative lifecycle without changing it.
     * @return Current state; synchronous calculations do not affect this value.
     */
    CalculationStatus_E getStatus();




    /**
     * @brief Calculate CRC-8/AUTOSAR (H2F) for a complete message or one block.
     * @details Polynomial 0x2F, initial value 0xFF, final XOR 0xFF.
     * The check value for the nine ASCII bytes "123456789" is 0xDF.
     * Input and output are not reflected.
     * @param[in] pData Readable input buffer; may be null only if dataLen is zero.
     * @param[in] dataLen Number of bytes to process; no string terminator is implied.
     * @param[in] firstCall True starts a new message using the fixed initial value.
     * False continues the checksum supplied in startValue.
     * @param[in] startValue Previous finalized checksum when firstCall is false;
     * ignored when firstCall is true. Only the low 8 bits are used.
     * @return Finalized 8-bit checksum. Returns zero for null input with nonzero
     * length or when CRC8H2F_ENABLED is CRC_DISABLED.
     * @note Empty first blocks return zero; empty continuation blocks return
     * the previous checksum truncated to the algorithm width. Zero can be a valid
     * result and cannot be used alone to detect invalid input.
     * @note Does not modify any Crc instance or the supplied data.
     */
    static uint8_t calculateCrc8H2F(const uint8_t* pData, uint32_t dataLen,
        bool firstCall = true, uint32_t startValue = CRC_START_VALUE);
    /**
     * @brief Calculate CRC-8/SAE-J1850 for a complete message or one block.
     * @details Polynomial 0x1D, initial value 0xFF, final XOR 0xFF.
     * The check value for the nine ASCII bytes "123456789" is 0x4B.
     * Input and output are not reflected.
     * @param[in] pData Readable input buffer; may be null only if dataLen is zero.
     * @param[in] dataLen Number of bytes to process; no string terminator is implied.
     * @param[in] firstCall True starts a new message using the fixed initial value.
     * False continues the checksum supplied in startValue.
     * @param[in] startValue Previous finalized checksum when firstCall is false;
     * ignored when firstCall is true. Only the low 8 bits are used.
     * @return Finalized 8-bit checksum. Returns zero for null input with nonzero
     * length or when CRC8_ENABLED is CRC_DISABLED.
     * @note Empty first blocks return zero; empty continuation blocks return
     * the previous checksum truncated to the algorithm width. Zero can be a valid
     * result and cannot be used alone to detect invalid input.
     * @note Does not modify any Crc instance or the supplied data.
     */
    static uint8_t calculateCrc8(const uint8_t* pData, uint32_t dataLen,
        bool firstCall = true, uint32_t startValue = CRC_START_VALUE);
    /**
     * @brief Calculate CRC-16/IBM-3740 (CCITT-FALSE) for a complete message or one block.
     * @details Polynomial 0x1021, initial value 0xFFFF, final XOR 0.
     * The check value for the nine ASCII bytes "123456789" is 0x29B1.
     * Input and output are not reflected.
     * @param[in] pData Readable input buffer; may be null only if dataLen is zero.
     * @param[in] dataLen Number of bytes to process; no string terminator is implied.
     * @param[in] firstCall True starts a new message using the fixed initial value.
     * False continues the checksum supplied in startValue.
     * @param[in] startValue Previous finalized checksum when firstCall is false;
     * ignored when firstCall is true. Only the low 16 bits are used.
     * @return Finalized 16-bit checksum. Returns zero for null input with nonzero
     * length or when CRC16_ENABLED is CRC_DISABLED.
     * @note Empty first blocks return 0xFFFF; empty continuation blocks return
     * the previous checksum truncated to the algorithm width. Zero can be a valid
     * result and cannot be used alone to detect invalid input.
     * @note Does not modify any Crc instance or the supplied data.
     */
    static uint16_t calculateCrc16(const uint8_t* pData, uint32_t dataLen,
        bool firstCall = true, uint32_t startValue = CRC_START_VALUE);
    /**
     * @brief Calculate CRC-32/ISO-HDLC for a complete message or one block.
     * @details Polynomial 0x04C11DB7, initial value 0xFFFFFFFF, final XOR 0xFFFFFFFF.
     * The check value for the nine ASCII bytes "123456789" is 0xCBF43926.
     * Input and output are reflected; the implementation uses polynomial 0xEDB88320.
     * @param[in] pData Readable input buffer; may be null only if dataLen is zero.
     * @param[in] dataLen Number of bytes to process; no string terminator is implied.
     * @param[in] firstCall True starts a new message using the fixed initial value.
     * False continues the checksum supplied in startValue.
     * @param[in] startValue Previous finalized checksum when firstCall is false;
     * ignored when firstCall is true. Only the low 32 bits are used.
     * @return Finalized 32-bit checksum. Returns zero for null input with nonzero
     * length or when CRC32_ENABLED is CRC_DISABLED.
     * @note Empty first blocks return zero; empty continuation blocks return
     * the previous checksum truncated to the algorithm width. Zero can be a valid
     * result and cannot be used alone to detect invalid input.
     * @note Does not modify any Crc instance or the supplied data.
     */
    static uint32_t calculateCrc32(const uint8_t* pData, uint32_t dataLen,
        bool firstCall = true, uint32_t startValue = CRC_START_VALUE);
    /**
     * @brief Dispatch a synchronous block calculation to the selected algorithm.
     * @param[in] type Algorithm to use for every block of the same message.
     * @param[in] pData Readable input buffer, or null for a zero-length block.
     * @param[in] dataLen Number of bytes in this block.
     * @param[in] firstCall True starts a new message; false continues a prior result.
     * @param[in] startValue Finalized return value from the previous block. Ignored
     * on the first call; high bits outside the algorithm width are discarded.
     * @return Finalized CRC, zero-extended to 32 bits. Returns zero for an unknown
     * or disabled type, or for a null pointer with nonzero length.
     * @details Empty first blocks produce 0xFFFF for CRC_16 and zero for the other
     * algorithms. Empty continuation blocks preserve the previous checksum within
     * its width. No cooperative state is read or changed.
     * @warning The caller must supply a buffer of at least dataLen bytes; the library
     * cannot verify the allocated buffer size. Zero is not a unique error indicator.
     * @code{.cpp}
     * const uint8_t bytes[] = "123456789";
     * uint32_t crc = Crc::calculate(CRC_16, bytes, 4);
     * crc = Crc::calculate(CRC_16, bytes + 4, 5, false, crc); // 0x29B1
     * @endcode
     */
    static uint32_t calculate(Algorithm_E type, const uint8_t* pData, uint32_t dataLen,
        bool firstCall = true, uint32_t startValue = CRC_START_VALUE);

private:
    friend class CrcIf<Crc>;
    /** @brief Check the selected compile-time enable flag. @return False for unknown types. */
    bool isAlgorithmEnabled() const;
    /**
     * @brief Route a block through the enum dispatcher using direct static calls.
     * @param[in] pData Readable input buffer, or null for an empty block.
     * @param[in] dataLen Number of bytes to process.
     * @param[in] startValue Previous finalized CRC, ignored for firstCall=true.
     * @param[in] firstCall True starts a message; false continues the previous result.
     * @return Finalized CRC, or zero for invalid input or an unavailable algorithm.
     */
    uint32_t calculateBlock(const uint8_t* pData, uint32_t dataLen,
                            uint32_t startValue, bool firstCall) const;
private:
    Algorithm_E mType; ///< Runtime algorithm selection; processing state lives in CrcIf.
};
#endif
