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



/** \brief Number of elements in CRC8 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC_8_TABLE_SIZE      256U

class Crc8
{
   public:
      Crc8();
      virtual ~Crc8();
};

#endif /* SOUCRE_CRC_CRC8_H_ */
