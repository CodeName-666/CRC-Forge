/*
 * Crc8H2F.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC8H2F_H_
#define _CRC8H2F_H_

#include "CrcIf.h"

/** @brief CRC8 0x2F polynomial */
#define CRC8H2F_POLYNOMIAL               0x2FU


#define CRC8H2F_TABLE_SIZE             CRC_LARGE_TABLE_CALCULATION


#if !defined(CRC8H2F_TABLE_SIZE)
/**
 * \brief Number of elements in CRC8H2F lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC8H2F_TABLE_SIZE             CRC_LARGE_TABLE_CALCULATION
#endif




class Crc8H2F : public CrcIf
{
   public:
      Crc8H2F();
      virtual ~Crc8H2F();
      uint32_t calculate(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = CRC8_INITIAL_VALUE, boolean isFirstCall = true);
   private:
      uint32_t calculateToRunntime    (uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = CRC8_INITIAL_VALUE, boolean isFirstCall = true );
      uint32_t calculateWithSmallTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = CRC8_INITIAL_VALUE, boolean isFirstCall = true );
      uint32_t calculateWithLargeTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue = CRC8_INITIAL_VALUE, boolean isFirstCall = true );

};


#endif /* SOUCRE_CRC_CRC8H2F_H_ */
