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



class Crc8H2F
{
   public:
      Crc8H2F();
      virtual ~Crc8H2F();
};

#endif /* SOUCRE_CRC_CRC8H2F_H_ */
