/*
* Crc.cpp
*
*  Created on: 22.10.2017
*      Author: AP02
*/

#include "Crc.h"
#include "Crc8.h"
#include "Crc8H2F.h"
#include "Crc16.h"
#include "Crc32.h"

Crc::Crc()
{
// TODO Auto-generated constructor stub

}

Crc::~Crc()
{
// TODO Auto-generated destructor stub
}



uint32 Crc::getDataLen() const
{
   return _dataLen;
}

void Crc::setDataLen(uint32 dataLen)
{
   _dataLen = dataLen;
}

uint8_t* Crc::getDataPtr() const
{
   return _dataPtr;
}

void Crc::setDataPtr(uint8_t* dataPtr)
{
   _dataPtr = dataPtr;
}

Crc::Crc_t Crc::getType() const
{
   return _type;
}

void Crc::setType(Crc_t type)
{
   _type = type;
}


uint32_t Crc::calculate(void)
{
   return  Crc::calculate(_type, _dataPtr, _dataLen);

}

uint8_t  Crc::calculateCrc82HF(uint8_t* dataPtr, uint32_t dataLen)
{
   uint8_t ret = 0;
#if (CRC8H2F_ENABLED == 1)
   ret =  Crc8H2F::calculate(dataPtr,dataLen,0,true);
#endif
   return ret;
}

uint8_t  Crc::calculateCrc8(uint8_t* dataPtr, uint32_t dataLen)
{
   uint8_t ret = 0;
#if (CRC8_ENABLED == 1)
   ret =  Crc8::calculate(dataPtr,dataLen,0,true);
#endif
   return ret;
}

uint16_t Crc::calculateCrc16(uint8_t* dataPtr, uint32_t dataLen)
{
   uint16_t ret = 0;
#if (CRC16_ENABLED == 1)
   ret = Crc16::calculate(dataPtr,dataLen,0,true);
#endif
   return ret;
}

uint32_t Crc::calculateCrc32(uint8_t* dataPtr, uint32_t dataLen)
{
   uint32_t ret = 0;
#if (CRC32_ENABLED == 1)
   ret = Crc32::calculate(dataPtr,dataLen,0,true);
#endif
   return ret;
}


uint32_t Crc::calculate(Crc_t type, uint8_t* dataPtr, uint32_t dataLen)
{
   uint32_t ret = 0;
    if(dataPtr != NULL && dataLen > 0u)
    {
       switch(type)
       {
          case CRC_8:
#if (CRC8_ENABLED == 1)
             ret = (uint32_t)Crc8::calculate(dataPtr,dataLen,0,true);
#endif
             break;
          case CRC_8H2F:
#if (CRC8H2F_ENABLED == 1)
             ret = (uint32_t)Crc8H2F::calculate(dataPtr,dataLen,0,true);
#endif
             break;
          case CRC16:
#if (CRC16_ENABLED == 1)
             ret = (uint32_t)Crc16::calculate(dataPtr,dataLen,0,true);
#endif
             break;
          case CRC32:
#if (CRC32_ENABLED == 1)
             ret = (uint32_t)Crc32::calculate(dataPtr,dataLen,0,true);
#endif
             break;
          default:
             break;
       }
    }
    return ret;
}
