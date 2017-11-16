/*
 * Crc8.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC8_H_
#define _CRC8_H_

#include "CrcIf.h"


/** @brief SAE J1850 CRC8 polynomial
 *
 * According to AUTOSAR R4.0 CRC SWS CRC030
 */
#define CRC8_POLYNOMIAL                                        0x1DU


#define CRC8_TABLE_SIZE                CRC_LARGE_TABLE_CALCULATION


#if !defined(CRC8_TABLE_SIZE)
/**
 *  \brief Number of elements in CRC8 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC8_TABLE_SIZE                CRC_LARGE_TABLE_CALCULATION
#endif

class Crc8 : public CrcIf
{
   public:
      Crc8();
      virtual ~Crc8();
      uint32_t calculate(uint8* dataPtr, uint32 dataLength, uint8 startValue = CRC8_INITIAL_VALUE, boolean isFirstCall = true);
   private:
      uint32_t calculateToRunntime(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = true, boolean isFirstCall = CRC16_INITIAL_VALUE );
      uint32_t calculateWithSmallTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = true, boolean isFirstCall = CRC16_INITIAL_VALUE );
      uint32_t calculateWithLargeTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = true, boolean isFirstCall = CRC16_INITIAL_VALUE );

};
#endif



