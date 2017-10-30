/*
* Crc.cpp
*
*  Created on: 22.10.2017
*      Author: AP02
*/

#include "Crc.h"



Crc::Crc()
{
   _dataPtr   = NULL;
   _dataLen   = 0;
   _crc       = 0;
   _type      = CRC_8;
   _dataCount = 0;
   _status    = CRC_NO_CALC;
}

Crc::~Crc()
{
      _dataPtr   = NULL;
      _dataLen   = 0;
      _crc       = 0;
      _type      = CRC_8;
      _dataCount = 0;
      _status    = CRC_NO_CALC;
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
   return  Crc::calculate(_type, _dataPtr, _dataLen,true);

}

uint8_t  Crc::calculateCrc82HF(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint8_t ret = 0;
#if (CRC8H2F_ENABLED == 1)
   ret =  Crc8H2F::calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

uint8_t  Crc::calculateCrc8(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint8_t ret = 0;
#if (CRC8_ENABLED == 1)
   ret =  Crc8::calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

uint16_t Crc::calculateCrc16(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint16_t ret = 0;
#if (CRC16_ENABLED == 1)
   ret = Crc16::calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

uint32_t Crc::calculateCrc32(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint32_t ret = 0;
#if (CRC32_ENABLED == 1)
   ret = Crc32::calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}


uint32_t Crc::calculate(Crc_t type, uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint32_t ret = 0;
    if(dataPtr != NULL && dataLen > 0u)
    {
       switch(type)
       {
          case CRC_8:
#if (CRC8_ENABLED == 1)
             ret = (uint32_t)Crc8::calculate(dataPtr,dataLen,(uint8_t)startValue,firstCall);
#endif
             break;
          case CRC_8H2F:
#if (CRC8H2F_ENABLED == 1)
             ret = (uint32_t)Crc8H2F::calculate(dataPtr,dataLen,(uint8_t)startValue,firstCall);
#endif
             break;
          case CRC16:
#if (CRC16_ENABLED == 1)
             ret = (uint32_t)Crc16::calculate(dataPtr,dataLen,(uint16_t)startValue,firstCall);
#endif
             break;
          case CRC32:
#if (CRC32_ENABLED == 1)
             ret = (uint32_t)Crc32::calculate(dataPtr,dataLen,(uint32_t)startValue,firstCall);
#endif
             break;
          default:
             break;
       }
    }
    return ret;
}


boolean Crc::isFinished(void)
{
   return (_status == CRC_CALC_FINISHED) ? true : false;
}

uint32_t Crc::getCrc(void)
{
   uint32_t ret = 0;

   if(_status == CRC_CALC_FINISHED)
   {
      ret = _crc;
      _dataCount = 0;
      _status = CRC_NO_CALC;
   }
   return ret;
}

void Crc::loop(void)
{

   if(_status == CRC_CALC_ACTIVE)
   {
      if(_dataCount != 0)
      {
         _crc = calculate(_type,&(_dataPtr[_dataCount]),1,false,_crc);
      }
      else
      {
         _crc = calculate(_type,&(_dataPtr[_dataCount]),1,true,CRC_START_VALUE);
      }
      if(_dataCount < _dataLen)
      {
         _dataCount++;
      }
      else
      {
         _status = CRC_CALC_FINISHED;
      }
   }
   return;
}

Crc::Crc_CalcStatus_t Crc::getStatus(void)
{
   return _status;
}


void Crc::start(void)
{
   if(_dataPtr != NULL && _dataLen != 0)
   {
      if(_status == CRC_NO_CALC)
      {
         _status = CRC_CALC_ACTIVE;
         _dataCount = 0;
      }
   }
   return;
}
