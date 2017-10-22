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

#include "CrcCommon.h"


/** @brief Definition of the initial value of crc32 */
#define CRC32_INITIAL_VALUE   0xFFFFFFFFU

/** @brief definition of key width CRC32 polynomial [CRC002]
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



class Crc32
{
   public:
      Crc32();
      virtual ~Crc32();
      uint32_t calculate(uint8_t* Crc_DataPtr,
                         uint32_t Crc_Length,
                         uint32_t Crc_StartValue32,
                         boolean Crc_IsFirstCall);
};

#endif /* SOUCRE_CRC_CRC32_H_ */
