/**
 * @file Crc_Cfg.h
 * @brief Compile-time algorithm selection, backend configuration and table access.
 * @details Define configuration macros globally through compiler -D options
 * (PlatformIO build_flags) or edit their defaults here. All translation units
 * must use the same settings. Defines in application source alone do not
 * configure the separately compiled library.
 *
 * CRC_TABLE_SIZE sets the default backend; each CRCx_TABLE_SIZE can override
 * it independently. Values 0, 16 and 256 select bitwise CPU calculation,
 * nibble lookup tables and byte lookup tables. All algorithms and 256-entry
 * tables are enabled by default. Unsupported values cause compilation errors.
 *
 * With all four algorithms enabled, tables occupy 0, 128 or 2048 bytes for
 * the respective modes. These numbers exclude instructions and other data.
 * On AVR, tables reside in flash and are read with program-memory primitives.
 */
#ifndef CRC_CFG_H
#define CRC_CFG_H

#include <stdint.h>

/** @brief Default public API seed placeholder. Ignored on the first call;
 * continuation calls must supply the previous finalized CRC instead. */
#define CRC_START_VALUE 0xFFFFFFFFUL
/** @brief Bitwise CPU backend: eight polynomial steps per byte, no table. */
#define CRC_SYSTEM_CALCULATION 0U
/** @brief Nibble backend: 16 entries, two table accesses per input byte. */
#define CRC_SMALL_TABLE_CALCULATION 16U
/** @brief Byte backend: 256 entries, one table access per input byte. */
#define CRC_LARGE_TABLE_CALCULATION 256U
/** @brief Enable an algorithm at compile time. */
#define CRC_ENABLED 1U
/** @brief Exclude an algorithm implementation and its table at compile time. */
#define CRC_DISABLED 0U

#ifndef CRC_TABLE_SIZE
/** @brief Default backend for all algorithms unless individually overridden.
 * Valid values are 0, 16 and 256; defaults to CRC_LARGE_TABLE_CALCULATION. */
#define CRC_TABLE_SIZE CRC_LARGE_TABLE_CALCULATION
#endif
#if CRC_TABLE_SIZE != 0 && CRC_TABLE_SIZE != 16 && CRC_TABLE_SIZE != 256
#error "CRC_TABLE_SIZE must be 0, 16 or 256"
#endif

/** @brief Enable CRC8 with 1 or exclude it with 0; accepts the legacy alias. */
#ifndef CFG_CRC8_ENABLE
#ifdef CRC8_ENABLED
#define CFG_CRC8_ENABLE CRC8_ENABLED
#else
#define CFG_CRC8_ENABLE CRC_ENABLED
#endif
#endif
#ifndef CRC8_ENABLED
#define CRC8_ENABLED CFG_CRC8_ENABLE
#endif
#if CFG_CRC8_ENABLE != CRC8_ENABLED
#error "Conflicting CRC8 enable definitions"
#endif

/** @brief Enable CRC8H2F with 1 or exclude it with 0; accepts the legacy alias. */
#ifndef CFG_CRC8H2F_ENABLE
#ifdef CRC8H2F_ENABLED
#define CFG_CRC8H2F_ENABLE CRC8H2F_ENABLED
#else
#define CFG_CRC8H2F_ENABLE CRC_ENABLED
#endif
#endif
#ifndef CRC8H2F_ENABLED
#define CRC8H2F_ENABLED CFG_CRC8H2F_ENABLE
#endif
#if CFG_CRC8H2F_ENABLE != CRC8H2F_ENABLED
#error "Conflicting CRC8H2F enable definitions"
#endif

/** @brief Enable CRC16 with 1 or exclude it with 0; accepts the legacy alias. */
#ifndef CFG_CRC16_ENABLE
#ifdef CRC16_ENABLED
#define CFG_CRC16_ENABLE CRC16_ENABLED
#else
#define CFG_CRC16_ENABLE CRC_ENABLED
#endif
#endif
#ifndef CRC16_ENABLED
#define CRC16_ENABLED CFG_CRC16_ENABLE
#endif
#if CFG_CRC16_ENABLE != CRC16_ENABLED
#error "Conflicting CRC16 enable definitions"
#endif

/** @brief Enable CRC32 with 1 or exclude it with 0; accepts the legacy alias. */
#ifndef CFG_CRC32_ENABLE
#ifdef CRC32_ENABLED
#define CFG_CRC32_ENABLE CRC32_ENABLED
#else
#define CFG_CRC32_ENABLE CRC_ENABLED
#endif
#endif
#ifndef CRC32_ENABLED
#define CRC32_ENABLED CFG_CRC32_ENABLE
#endif
#if CFG_CRC32_ENABLE != CRC32_ENABLED
#error "Conflicting CRC32 enable definitions"
#endif

#ifndef CRC8_ENABLED
/** @brief Enable CRC8 with 1, or disable it with 0; default is enabled.
 * Disabled algorithms return zero through Crc wrappers and cannot be started
 * cooperatively. Their direct algorithm methods are not linked. */
#define CRC8_ENABLED CRC_ENABLED
#endif
#if CRC8_ENABLED != CRC_ENABLED && CRC8_ENABLED != CRC_DISABLED
#error "CRC8_ENABLED must be 0 or 1"
#endif
#ifndef CRC8_TABLE_SIZE
/** @brief Backend for CRC8: 0 (CPU), 16 (nibble table), or 256 (byte table).
 * Defaults to CRC_TABLE_SIZE; overrides the global backend for this algorithm. */
#define CRC8_TABLE_SIZE CRC_TABLE_SIZE
#endif
#if CRC8_TABLE_SIZE != 0 && CRC8_TABLE_SIZE != 16 && CRC8_TABLE_SIZE != 256
#error "CRC8_TABLE_SIZE must be 0, 16 or 256"
#endif

#ifndef CRC8H2F_ENABLED
/** @brief Enable CRC8H2F with 1, or disable it with 0; default is enabled.
 * Disabled algorithms return zero through Crc wrappers and cannot be started
 * cooperatively. Their direct algorithm methods are not linked. */
#define CRC8H2F_ENABLED CRC_ENABLED
#endif
#if CRC8H2F_ENABLED != CRC_ENABLED && CRC8H2F_ENABLED != CRC_DISABLED
#error "CRC8H2F_ENABLED must be 0 or 1"
#endif
#ifndef CRC8H2F_TABLE_SIZE
/** @brief Backend for CRC8H2F: 0 (CPU), 16 (nibble table), or 256 (byte table).
 * Defaults to CRC_TABLE_SIZE; overrides the global backend for this algorithm. */
#define CRC8H2F_TABLE_SIZE CRC_TABLE_SIZE
#endif
#if CRC8H2F_TABLE_SIZE != 0 && CRC8H2F_TABLE_SIZE != 16 && CRC8H2F_TABLE_SIZE != 256
#error "CRC8H2F_TABLE_SIZE must be 0, 16 or 256"
#endif

#ifndef CRC16_ENABLED
/** @brief Enable CRC16 with 1, or disable it with 0; default is enabled.
 * Disabled algorithms return zero through Crc wrappers and cannot be started
 * cooperatively. Their direct algorithm methods are not linked. */
#define CRC16_ENABLED CRC_ENABLED
#endif
#if CRC16_ENABLED != CRC_ENABLED && CRC16_ENABLED != CRC_DISABLED
#error "CRC16_ENABLED must be 0 or 1"
#endif
#ifndef CRC16_TABLE_SIZE
/** @brief Backend for CRC16: 0 (CPU), 16 (nibble table), or 256 (byte table).
 * Defaults to CRC_TABLE_SIZE; overrides the global backend for this algorithm. */
#define CRC16_TABLE_SIZE CRC_TABLE_SIZE
#endif
#if CRC16_TABLE_SIZE != 0 && CRC16_TABLE_SIZE != 16 && CRC16_TABLE_SIZE != 256
#error "CRC16_TABLE_SIZE must be 0, 16 or 256"
#endif

#ifndef CRC32_ENABLED
/** @brief Enable CRC32 with 1, or disable it with 0; default is enabled.
 * Disabled algorithms return zero through Crc wrappers and cannot be started
 * cooperatively. Their direct algorithm methods are not linked. */
#define CRC32_ENABLED CRC_ENABLED
#endif
#if CRC32_ENABLED != CRC_ENABLED && CRC32_ENABLED != CRC_DISABLED
#error "CRC32_ENABLED must be 0 or 1"
#endif
#ifndef CRC32_TABLE_SIZE
/** @brief Backend for CRC32: 0 (CPU), 16 (nibble table), or 256 (byte table).
 * Defaults to CRC_TABLE_SIZE; overrides the global backend for this algorithm. */
#define CRC32_TABLE_SIZE CRC_TABLE_SIZE
#endif
#if CRC32_TABLE_SIZE != 0 && CRC32_TABLE_SIZE != 16 && CRC32_TABLE_SIZE != 256
#error "CRC32_TABLE_SIZE must be 0, 16 or 256"
#endif


#ifdef __AVR__
#include <avr/pgmspace.h>
/** @brief Table storage qualifier: PROGMEM on AVR, empty on other targets. */
#define CRC_TABLE_STORAGE PROGMEM
/**
 * @brief Read an 8-bit entry from the selected table storage.
 * @param value Addressable table element, such as table[index].
 * @return Entry value read from flash on AVR or ordinary memory elsewhere.
 * @pre The table was declared with CRC_TABLE_STORAGE.
 */
#define M_CRC_READ8(value) (pgm_read_byte(&(value)))
/**
 * @brief Read a 16-bit entry from the selected table storage.
 * @param value Addressable table element, such as table[index].
 * @return Entry value read from flash on AVR or ordinary memory elsewhere.
 * @pre The table was declared with CRC_TABLE_STORAGE.
 */
#define M_CRC_READ16(value) (pgm_read_word(&(value)))
/**
 * @brief Read a 32-bit entry from the selected table storage.
 * @param value Addressable table element, such as table[index].
 * @return Entry value read from flash on AVR or ordinary memory elsewhere.
 * @pre The table was declared with CRC_TABLE_STORAGE.
 */
#define M_CRC_READ32(value) (pgm_read_dword(&(value)))
#else
/** @brief Table storage qualifier: PROGMEM on AVR, empty on other targets. */
#define CRC_TABLE_STORAGE
/**
 * @brief Read an 8-bit entry from the selected table storage.
 * @param value Addressable table element, such as table[index].
 * @return Entry value read from flash on AVR or ordinary memory elsewhere.
 * @pre The table was declared with CRC_TABLE_STORAGE.
 */
#define M_CRC_READ8(value) (value)
/**
 * @brief Read a 16-bit entry from the selected table storage.
 * @param value Addressable table element, such as table[index].
 * @return Entry value read from flash on AVR or ordinary memory elsewhere.
 * @pre The table was declared with CRC_TABLE_STORAGE.
 */
#define M_CRC_READ16(value) (value)
/**
 * @brief Read a 32-bit entry from the selected table storage.
 * @param value Addressable table element, such as table[index].
 * @return Entry value read from flash on AVR or ordinary memory elsewhere.
 * @pre The table was declared with CRC_TABLE_STORAGE.
 */
#define M_CRC_READ32(value) (value)
#endif


/** @brief Fixed initial remainder for CRC-8/SAE-J1850 and CRC-8/AUTOSAR.
 * Both algorithms also use 0xFF as their final XOR value. */
#define CRC8_INITIAL_VALUE                                       0xFFU


/** @brief Fixed initial remainder for CRC-16/IBM-3740 (CCITT-FALSE).
 * Its final XOR is zero; continuation does not invert the previous checksum. */
#define CRC16_INITIAL_VALUE                                    0xFFFFU


/** @brief Fixed initial remainder for CRC-32/ISO-HDLC.
 * The final XOR is also 0xFFFFFFFF; continuation first reverses that XOR. */
#define CRC32_INITIAL_VALUE                                 0xFFFFFFFFU

/**
 * @brief Unsigned CRC xor operation.
 * @param[in] left First operand; evaluated once.
 * @param[in] right Second operand; evaluated once.
 * @return Operation result; narrow explicitly to the CRC width when storing.
 * @pre Shift counts, where applicable, are less than 32.
 */
#define M_CRC_XOR(left, right) ((left) ^ (right))

/**
 * @brief Unsigned CRC and operation.
 * @param[in] left First operand; evaluated once.
 * @param[in] right Second operand; evaluated once.
 * @return Operation result; narrow explicitly to the CRC width when storing.
 * @pre Shift counts, where applicable, are less than 32.
 */
#define M_CRC_AND(left, right) ((left) & (right))

/**
 * @brief Unsigned CRC shift left operation.
 * @param[in] value First operand; evaluated once.
 * @param[in] count Second operand; evaluated once.
 * @return Operation result; narrow explicitly to the CRC width when storing.
 * @pre Shift counts, where applicable, are less than 32.
 */
#define M_CRC_SHIFT_LEFT(value, count) (static_cast<uint32_t>(value) << (count))

/**
 * @brief Unsigned CRC shift right operation.
 * @param[in] value First operand; evaluated once.
 * @param[in] count Second operand; evaluated once.
 * @return Operation result; narrow explicitly to the CRC width when storing.
 * @pre Shift counts, where applicable, are less than 32.
 */
#define M_CRC_SHIFT_RIGHT(value, count) (static_cast<uint32_t>(value) >> (count))

/** @brief Legacy table-read alias. @param[in] value Addressable table entry.
 * @return Entry value. @see M_CRC_READ8 */
#define CRC_READ8(value) (M_CRC_READ8(value))

/** @brief Legacy table-read alias. @param[in] value Addressable table entry.
 * @return Entry value. @see M_CRC_READ16 */
#define CRC_READ16(value) (M_CRC_READ16(value))

/** @brief Legacy table-read alias. @param[in] value Addressable table entry.
 * @return Entry value. @see M_CRC_READ32 */
#define CRC_READ32(value) (M_CRC_READ32(value))

#endif
