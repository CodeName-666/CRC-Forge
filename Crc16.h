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



/** \brief Number of elements in CRC16 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC_16_TABLE_SIZE     256U

class Ccr16
{
   public:
      Ccr16();
      virtual ~Ccr16();
};

#endif /* SOUCRE_CRC_CCR16_H_ */
