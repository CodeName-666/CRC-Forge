/*
 * Crc16.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC16_H_
#define _CRC16_H_

#include "CrcIf.h"

#define CRC16_SYSTEM_CALCULATION                                  0U
#define CRC16_SMALL_TABLE_CALCULATION                            16U
#define CRC16_LARGE_TABLE_CALCULATION                           256U



/**
 * @brief Definition of the initial value of crc16
 */
#define CRC16_INITIAL_VALUE   0xFFFFU

/**
 * @brief definition of key width CRC16 polynomial [CRC002]
 */
#define CRC16_POLYNOMIAL    0x1021U


#define CRC16_TABLE_SIZE               CRC16_LARGE_TABLE_CALCULATION

#if !defined(CRC16_TABLE_SIZE)
/**
 * @brief Number of elements in CRC16 lookup table
 *
 * If size is 0 table based calculation is deactivated.
 */
#define CRC16_TABLE_SIZE     CRC16_LARGE_TABLE_CALCULATION
#endif






class Crc16 : public Crc16If
{
   public:
      Crc16();
      virtual ~Crc16();
      uint16_t calculate(uint8_t* dataPtr,
                                uint32_t dataLength,
                                uint16_t startValue,
                                boolean isFirstCall
                               );
};


#endif /* _CRC16_H_ */
