/*
 * Crc32.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC32_H_
#define _CRC32_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif


#define CRC32_SYSTEM_CALCULATION                                   0U
#define CRC32_SMALL_TABLE_CALCULATION                             16U
#define CRC32_LARGE_TABLE_CALCULATION                            256U


/**
 * @brief Definition of the initial value of crc32
 */
#define CRC32_INITIAL_VALUE   0xFFFFFFFFU

/**
 * @brief definition of key width CRC32 polynomial [CRC002]
 *
 *The CRC32 routine is based on IEEE-802.3 CRC32 Ethernet standard.
 *In there, the polynomial 0x04C11DB7 is specified to be used.
 *
 *In that standard, the reflection of all input bytes is specified.
 *We use an optimized algorithm where we do not reflect the input
 *but the polynomial.
 *So the polynomial 0xEDB88320 specified below is the reflected
 *polynomial of the polynomial 0x04C11DB7.
 *
 *See the "A Painless Guide to CRC Error Detection Algorithms",
 *R. Williams, 1993.
 */
#define CRC32_POLYNOMIAL    0xEDB88320U


#define CRC32_TABLE_SIZE               CRC32_LARGE_TABLE_CALCULATION


#if !defined(CRC32_TABLE_SIZE)
/**
 * @brief Number of elements in CRC32 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC32_TABLE_SIZE               CRC32_LARGE_TABLE_CALCULATION
#endif


class Crc32
{
   public:
      Crc32();
      virtual ~Crc32();
      static uint32_t calculate(uint8_t* dataPtr,
                                 uint32_t dataLength,
                                 uint32_t startValue,
                                 boolean isFirstCall
                                );
};


#endif /* SOUCRE_CRC_CRC32_H_ */
