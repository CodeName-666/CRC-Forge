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


class CrcIf
{
   public:
      CrcIf() {};
      virtual ~CrcIf() {};
      virtual uint32_t calculate(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall) {return 0;}
};



#endif /* SOUCRE_CRC_SRC_CRCIF_H_ */
