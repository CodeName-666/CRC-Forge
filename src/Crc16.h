/*
 * Crc16.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC16_H_
#define _CRC16_H_

#include "CrcIf.h"

/**
 * @brief definition of key width CRC16 polynomial [CRC002]
 */
#define CRC16_POLYNOMIAL    0x1021U


#define CRC16_TABLE_SIZE               CRC_LARGE_TABLE_CALCULATION

#if !defined(CRC16_TABLE_SIZE)
/**
 * @brief Number of elements in CRC16 lookup table
 *
 * If size is 0 table based calculation is deactivated.
 */
#define CRC16_TABLE_SIZE     CRC_LARGE_TABLE_CALCULATION
#endif






class Crc16 : public CrcIf
{
   public:
      Crc16();
      virtual ~Crc16();
      uint32_t calculate(uint8_t* dataPtr,
                                uint32_t dataLength,
                                uint16_t startValue,
                                boolean isFirstCall
                               );
};


#endif /* _CRC16_H_ */
