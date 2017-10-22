/*
 * Crc16.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC16_H_
#define _CRC16_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif



/**
 * @brief Definition of the initial value of crc16
 */
#define CRC16_INITIAL_VALUE   0xFFFFU

/**
 * @brief definition of key width CRC16 polynomial [CRC002]
 */
#define CRC16_POLYNOMIAL    0x1021U

#if !defined(CRC_16_TABLE_SIZE)
/**
 * @brief Number of elements in CRC16 lookup table
 *
 * If size is 0 table based calculation is deactivated.
 */
#define CRC16_TABLE_SIZE     256U
#endif



class Crc16
{
   public:
      Crc16();
      virtual ~Crc16();
      static uint16_t calculate(uint8_t* Crc_DataPtr,
                                uint32_t Crc_Length,
                                uint16_t Crc_StartValue16,
                                boolean Crc_IsFirstCall);
};

#endif /* _CRC16_H_ */
