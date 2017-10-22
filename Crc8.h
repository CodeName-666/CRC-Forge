/*
 * Crc8.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC8_H_
#define _CRC8_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif



/** @brief Definition of the initial value of the SAE J1850 CRC8 */
#define CRC8_INITIAL_VALUE        0xFFU

/** @brief SAE J1850 CRC8 polynomial
 *
 * According to AUTOSAR R4.0 CRC SWS CRC030
 */
#define CRC8_POLYNOMIAL          0x1DU

#if !defined(CRC_8_TABLE_SIZE)
/**
 *  \brief Number of elements in CRC8 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC8_TABLE_SIZE      256U
#endif

class Crc8
{
   public:
      Crc8();
      virtual ~Crc8();
      static uint8_t calculate(uint8_t* Crc_DataPtr,
                               uint32_t Crc_Length,
                               uint8_t Crc_StartValue8,
                               boolean Crc_IsFirstCall);
};

#endif /* SOUCRE_CRC_CRC8_H_ */
