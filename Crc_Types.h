/**
 * @file Crc_Types.h
 * @brief Shared configuration and cooperative calculation types.
 * @details Defines the data layout used by all CRC algorithms and the bridge.
 * It contains no calculation logic, inheritance, virtual functions or storage
 * allocation. Each algorithm owns its own CalculationState_T instance.
 * Compile-time switches and platform access macros remain in Crc_Cfg.h.
 */
#ifndef CRC_TYPES_H
#define CRC_TYPES_H

#include <stdint.h>

/** @brief Runtime algorithm identifiers with an explicit one-byte representation. */
enum Algorithm_E : uint8_t {
    CRC_8 = 0U, ///< CRC-8/SAE-J1850.
    CRC_8H2F,   ///< CRC-8/AUTOSAR.
    CRC_16,     ///< CRC-16/IBM-3740.
    CRC_32,     ///< CRC-32/ISO-HDLC.
    CRC_8_SMBUS = 4U ///< CRC-8/SMBUS: polynomial 0x07, init/xor-out zero.
};

/**
 * @brief Common lifecycle for every cooperative CRC calculation.
 * @details Used directly by the bridge and every algorithm class.
 */
enum CalculationStatus_E : uint8_t {
    CRC_NO_CALC = 0,   ///< Idle; a valid nonempty buffer may be started.
    CRC_CALC_ACTIVE,   ///< Active; each loop() call processes one byte.
    CRC_CALC_FINISHED  ///< Completed; get() consumes the pending result.
};

/**
 * @brief Borrowed input configuration shared by algorithms and the bridge.
 * @details The buffer is never copied, modified or freed by the library.
 * It must remain readable and unchanged while processing. A null pointer is
 * accepted for synchronous empty blocks; cooperative start() rejects null or
 * empty buffers. The owner of this structure performs validation.
 */
struct BufferConfiguration_T {
    uint8_t* pData; ///< Borrowed address of at least dataLen readable bytes.
    uint32_t dataLen; ///< Configured number of bytes, excluding any implicit terminator.
};

/**
 * @brief Per-instance state with the same layout for all CRC algorithms.
 * @details Aggregate initialization must explicitly supply configuration,
 * checksum, processed count and status. There are no hidden defaults or heap
 * allocations. Trivial copy/destruction preserves ordinary value semantics.
 * Copying preserves progress and shares the caller-owned input buffer only.
 * CrcIf owns transitions and validation; concrete algorithms perform calculation.
 */
struct CalculationState_T {
    BufferConfiguration_T configuration; ///< Input pointer and total byte count.
    uint32_t checksum; ///< Finalized CRC of the bytes processed so far.
    uint32_t processedBytes; ///< Cooperative progress; never exceeds the configured length.
    CalculationStatus_E status; ///< Lifecycle associated with this instance's checksum.
};


#endif
