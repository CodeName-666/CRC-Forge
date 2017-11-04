/*
 * Crc8H2F.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC8H2F_H_
#define _CRC8H2F_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif


#define CRC8H2F_SYSTEM_CALCULATION                                   0U
#define CRC8H2F_SMALL_TABLE_CALCULATION                            16U
#define CRC8H2F_LARGE_TABLE_CALCULATION                           256U


/** @brief Definition of the initial value of the CRC8 on polynom 0x2F */
#define CRC8H2F_INITIAL_VALUE        0xFFU

/** @brief CRC8 0x2F polynomial */
#define CRC8H2F_POLYNOMIAL               0x2FU


#define CRC8H2F_TABLE_SIZE             CRC8H2F_LARGE_TABLE_CALCULATION


#if !defined(CRC8H2F_TABLE_SIZE)
/**
 * \brief Number of elements in CRC8H2F lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC8H2F_TABLE_SIZE             CRC8H2F_LARGE_TABLE_CALCULATION
#endif




class Crc8H2F
{
   public:
      Crc8H2F();
      virtual ~Crc8H2F();
      static uint8_t calculate(uint8_t* dataPtr,
                               uint32_t dataLength,
                               uint8_t startValue,
                               boolean isFirstCall);
};


#endif /* SOUCRE_CRC_CRC8H2F_H_ */
