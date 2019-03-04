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


#define CRC_SYSTEM_CALCULATION                                  0U
#define CRC_SMALL_TABLE_CALCULATION                            16U
#define CRC_LARGE_TABLE_CALCULATION                           256U



/** @brief Definition of the initial value of the CRC8 and CRC8H2F */
#define CRC8_INITIAL_VALUE                                       0xFFU

/**
 * @brief Definition of the initial value of crc16
 */
#define CRC16_INITIAL_VALUE                                    0xFFFFU

/**
 * @brief Definition of the initial value of crc32
 */
#define CRC32_INITIAL_VALUE                                 0xFFFFFFFFU



class CrcIf
{
   public:
      CrcIf();
      virtual ~CrcIf();
      virtual uint32_t calculate(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall) = 0;

   private:
      virtual uint32_t calculateToRunntime(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall)= 0;
      virtual uint32_t calculateWithSmallTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall)= 0;
      virtual uint32_t calculateWithLargeTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall)= 0;


   protected:
      uint32_t firstCall(boolean status, uint8_t startValue);
      uint32_t firstCall(boolean status, uint16_t startValue);
      uint32_t firstCall(boolean status, uint32_t startValue);

};



#endif /* SOUCRE_CRC_SRC_CRCIF_H_ */
