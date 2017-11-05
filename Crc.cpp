/*
* Crc.cpp
*
*  Created on: 22.10.2017
*      Author: AP02
*/

#include "Crc.h"
#include "src/CrcIf.h"


/******************************************************************************
 * FUNCTION: Crc(...)
 ******************************************************************************/
Crc::Crc()
{
   _dataPtr   = NULL;
   _dataLen   = 0u;
   _crc       = 0u;
   _type      = CRC_8;
   _dataCount = 0u;
   _status    = CRC_NO_CALC;
}

/******************************************************************************
 * FUNCTION: ~Crc(...)
 ******************************************************************************/
Crc::~Crc()
{
      _dataPtr   = NULL;
      _dataLen   = 0u;
      _crc       = 0u;
      _type      = CRC_8;
      _dataCount = 0u;
      _status    = CRC_NO_CALC;
}


/******************************************************************************
 * FUNCTION: uint32 getDataLen(...)
 ******************************************************************************/
uint32 Crc::getDataLen(void) const
{
   return _dataLen;
}

/******************************************************************************
 * FUNCTION: void setDataLen(...)
 ******************************************************************************/
void Crc::setDataLen(uint32 dataLen)
{
   _dataLen = dataLen;
}

/******************************************************************************
 * FUNCTION: uint8_t* getDataPtr(...)
 ******************************************************************************/
uint8_t* Crc::getDataPtr() const
{
   return _dataPtr;
}

/******************************************************************************
 * FUNCTION: void setDataPtr(...)
 ******************************************************************************/
void Crc::setDataPtr(uint8_t* dataPtr)
{
   _dataPtr = dataPtr;
}

/******************************************************************************
 * FUNCTION: Crc_t getType(...)
 ******************************************************************************/
Crc::Crc_t Crc::getType(void) const
{
   return _type;
}

/******************************************************************************
 * FUNCTION: void setType(...)
 ******************************************************************************/
void Crc::setType(Crc_t type)
{
   _type = type;
}

/******************************************************************************
 * FUNCTION: uint32_t calculate(...)
 ******************************************************************************/
uint32_t Crc::calculate(void)
{
   return Crc::calculate(_type, _dataPtr, _dataLen,true);

}

/******************************************************************************
 * FUNCTION: uint8_t calculateCrc82HF(...)
 ******************************************************************************/
uint8_t Crc::calculateCrc82HF(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint8_t ret = 0;
#if (CRC8H2F_ENABLED == 1)
   Crc8H2F crc;
   ret =  crc.calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

/******************************************************************************
 * FUNCTION: uint8_t calculateCrc8(...)
 ******************************************************************************/
uint8_t Crc::calculateCrc8(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint8_t ret = 0;
#if (CRC8_ENABLED == 1)
   Crc8 crc;
   ret =  crc.calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

/******************************************************************************
 * FUNCTION: uint16_t calculateCrc16(...)
 ******************************************************************************/
uint16_t Crc::calculateCrc16(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint16_t ret = 0;
#if (CRC16_ENABLED == 1)
   Crc16 crc;
   ret = crc.calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

/******************************************************************************
 * FUNCTION: uint32_t calculateCrc32(...)
 ******************************************************************************/
uint32_t Crc::calculateCrc32(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint32_t ret = 0;
#if (CRC32_ENABLED == 1)
   Crc32 crc;
   ret = crc.calculate(dataPtr,dataLen,startValue,firstCall);
#endif
   return ret;
}

/******************************************************************************
 * FUNCTION: uint32_t calculate(...)
 ******************************************************************************/
uint32_t Crc::calculate(Crc_t type, uint8_t* dataPtr, uint32_t dataLen, boolean firstCall, uint32 startValue)
{
   uint32_t ret = 0;
   CrcIf* crc;
    if(dataPtr != NULL && dataLen > 0u)
    {
       switch(type)
       {
          case CRC_8:
#if (CRC8_ENABLED == 1)
             Crc8 crc;
             ret = (uint32_t)crc.calculate(dataPtr,dataLen,(uint8_t)startValue,firstCall);
#endif
             break;
          case CRC_8H2F:
             Crc8H2F crc;
#if (CRC8H2F_ENABLED == 1)
            // ret = (uint32_t)Crc8H2F::calculate(dataPtr,dataLen,(uint8_t)startValue,firstCall);
#endif
             break;
          case CRC16:
#if (CRC16_ENABLED == 1)
             //ret = (uint32_t)Crc16::calculate(dataPtr,dataLen,(uint16_t)startValue,firstCall);
#endif
             break;
          case CRC32:
#if (CRC32_ENABLED == 1)
             //ret = (uint32_t)Crc32::calculate(dataPtr,dataLen,(uint32_t)startValue,firstCall);
#endif
             break;
          default:
             break;
       }
    }
    return ret;
}

/******************************************************************************
 * FUNCTION: boolean isFinished(...)
 ******************************************************************************/
boolean Crc::isFinished(void)
{
   return (_status == CRC_CALC_FINISHED) ? true : false;
}

/******************************************************************************
 * FUNCTION: uint32_t getCrc(...)
 ******************************************************************************/
uint32_t Crc::get(void)
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

/******************************************************************************
 * FUNCTION: void loop(...)
 ******************************************************************************/
void Crc::loop(void)
{

   if(_status == CRC_CALC_ACTIVE)
   {
      if(_dataCount < _dataLen)
      {
         if(_dataCount == 0)
         {
            _crc = calculate(_type,&(_dataPtr[_dataCount]),1,true,CRC_START_VALUE);
         }
         else
         {
            _crc = calculate(_type,&(_dataPtr[_dataCount]),1,false,_crc);
         }

         _dataCount++;
      }
      else
      {
         _status = CRC_CALC_FINISHED;
      }
   }
   return;
}

/******************************************************************************
 * FUNCTION: Crc_CalcStatus_t getStatus(...)
 ******************************************************************************/
Crc::Crc_CalcStatus_t Crc::getStatus(void)
{
   return _status;
}

/******************************************************************************
 * FUNCTION: boolean start(...)
 ******************************************************************************/
boolean Crc::start(void)
{
   boolean ret = false;
   if(_dataPtr != NULL && _dataLen != 0u)
   {
      if(_status == CRC_NO_CALC)
      {
         _status = CRC_CALC_ACTIVE;
         _dataCount = 0u;
         ret = true;
      }
   }
   return ret;
}

/******************************************************************************
 * FUNCTION: boolean cancle(...)
 ******************************************************************************/
boolean Crc::cancle(void)
{
   boolean ret = false;
   if(_dataPtr != NULL && _dataLen != 0u)
   {
      if(_status == CRC_CALC_ACTIVE)
      {
         _status = CRC_NO_CALC;
         _crc = 0u;
         _dataCount = 0u;
         ret = true;
      }
   }
   return ret;
}
