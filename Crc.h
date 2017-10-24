/*
 * Crc.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC_H_
#define _CRC_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif
#include "Crc_Cfg.h"


class Crc
{

   typedef enum
   {
      CRC_8          = 0x00,
      CRC_8H2F             ,
      CRC16                ,
      CRC32
   }Crc_t;


   public:
      Crc();
      virtual ~Crc();

      uint32_t getDataLen() const;
      uint8_t* getDataPtr() const;
      Crc_t getType() const;

      void setDataLen(uint32 dataLen);
      void setDataPtr(uint8_t* dataPtr);
      void setType(Crc_t type);

      uint32_t calculate(void);

      static uint8_t  calculateCrc82HF(uint8_t* dataPtr, uint32_t dataLen);
      static uint8_t  calculateCrc8(uint8_t* dataPtr, uint32_t dataLen);
      static uint16_t calculateCrc16(uint8_t* dataPtr, uint32_t dataLen);
      static uint32_t calculateCrc32(uint8_t* dataPtr, uint32_t dataLen);
      static uint32_t calculate(Crc_t type, uint8_t* dataPtr, uint32_t dataLen);
   private:
      uint8_t* _dataPtr;
      uint32_t _dataLen;
      Crc_t  _type;

};

#endif /* SOUCRE_CRC_CRC_H_ */
