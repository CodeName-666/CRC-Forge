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

#if (CRC8_ENABLED == CRC_ENABLED)
#include "src/Crc8.h"
#endif

#if(CRC8H2F_ENABLED == CRC_ENABLED)
#include "src/Crc8H2F.h"
#endif

#if(CRC16_ENABLED == CRC_ENABLED)
#include "src/Crc16.h"
#endif

#if(CRC32_ENABLED == CRC_ENABLED)
#include "src/Crc32.h"
#endif


class Crc
{

   typedef enum
   {
      CRC_8          = 0x00,
      CRC_8H2F             ,
      CRC16                ,
      CRC32
   }Crc_t;

   typedef enum
   {
      CRC_IMMEDIATE       =0x00,
      CRC_CYCLIC               ,
   }Crc_CalcType_t;

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

      boolean isFinished(void);
      uint32_t get(void);
      void loop(void);

   public: /*static functions*/
      static uint8_t  calculateCrc82HF(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE));
      static uint8_t  calculateCrc8   (uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE));
      static uint16_t calculateCrc16  (uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE));
      static uint32_t calculateCrc32  (uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE));
      static uint32_t calculate       (Crc_t type, uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE);
   private:
      uint8_t* _dataPtr;
      uint32_t _dataLen;
      Crc_t  _type;
      Crc_CalcType_t _calcType;
      uint32_t _dataCount;

};

#endif /* SOUCRE_CRC_CRC_H_ */
