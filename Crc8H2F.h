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



/** @brief Definition of the initial value of the CRC8 on polynom 0x2F */
#define CRC8H2F_INITIAL_VALUE        0xFFU

/** @brief CRC8 0x2F polynomial */
#define CRC8H2F_POLYNOMIAL               0x2FU

#if !defined(CRC_8H2F_TABLE_SIZE)
/**
 * \brief Number of elements in CRC8H2F lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC8H2F_TABLE_SIZE   256U
#endif


class Crc8H2F
{
   public:
      Crc8H2F();
      virtual ~Crc8H2F();
      static uint8_t calculate(uint8_t* Crc_DataPtr,
                               uint32_t Crc_Length,
                               uint8_t Crc_StartValue8H2F,
                               boolean Crc_IsFirstCall);
};

#endif /* SOUCRE_CRC_CRC8H2F_H_ */
