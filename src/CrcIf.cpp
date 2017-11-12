/*
 * CrcIf.c
 *
 *  Created on: 11.11.2017
 *      Author: AP02
 */


#include "CrcIf.h"



 CrcIf::CrcIf()
 {


 }


 CrcIf::~CrcIf()
 {

 }

uint32_t CrcIf::calculate(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall)
{

   return 0;
}


uint32_t CrcIf::firstCall(boolean status, uint8_t startValue)
{
   if(status == true)
   {
      startValue = CRC8_INITIAL_VALUE;
   }
   else
   {
      startValue ^= CRC8_INITIAL_VALUE;
   }
   return startValue;
}


uint32_t CrcIf::firstCall(boolean status, uint16_t startValue)
{
   if(status == true)
   {
      startValue = CRC16_INITIAL_VALUE;
   }
   else
   {
      startValue ^= CRC16_INITIAL_VALUE;
   }
   return startValue;
}


uint32_t CrcIf::firstCall(boolean status, uint32_t startValue)
{
   if(status == true)
   {
      startValue = CRC32_INITIAL_VALUE;
   }
   else
   {
      startValue ^= CRC32_INITIAL_VALUE;
   }
   return startValue;
}



uint32_t CrcIf::calculateToRunntime(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall);
uint32_t CrcIf::calculateWithSmallTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall);
uint32_t CrcIf::calculateWithLargeTabel(uint8_t* dataPtr, uint32_t dataLength, uint8_t startValue, boolean isFirstCall);


