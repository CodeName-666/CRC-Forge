/*
 * CrcIf.h
 *
 *  Created on: 04.11.2017
 *      Author: AP02
 */

#ifndef CRCIF_H_
#define CRCIF_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif


template < typename var1>

class CrcIf
{
   public:
      virtual CrcIf();
      virtual ~CrcIf();
      virtual var1 calculate(uint8_t* dataPtr, uint32_t dataLength, var1 startValue, boolean isFirstCall);
};


typedef CrcIf<uint8_t> Crc8If;
typedef CrcIf<uint8_t> Crc8H2FIf;
typedef CrcIf<uint16_t> Crc16If;
typedef CrcIf<uint32_t> Crc32If;

#endif /* SOUCRE_CRC_SRC_CRCIF_H_ */
