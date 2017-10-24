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

#define CRC_SYSTEM_CALCULATION                                   0U
#define CRC_SMALL_TABLE_CALCULATION                            16U
#define CRC_LARGE_TABLE_CALCULATION                           256U

#define CRC_ENABLED                                             1U
#define CRC_DISABLED                                            0U


#define CRC8_ENABLED                                   CRC_ENABLED
#define CRC8H2F_ENABLED                                CRC_ENABLED
#define CRC16_ENABLED                                  CRC_ENABLED
#define CRC32_ENABLED                                  CRC_ENABLED

#define CRC8_TABLE_SIZE                CRC_LARGE_TABLE_CALCULATION
#define CRC8H2F_TABLE_SIZE             CRC_LARGE_TABLE_CALCULATION
#define CRC16_TABLE_SIZE               CRC_LARGE_TABLE_CALCULATION
#define CRC32_TABLE_SIZE               CRC_LARGE_TABLE_CALCULATION

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
