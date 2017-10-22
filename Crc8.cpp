/*
 * Crc8.cpp
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#include "Crc8.h"



#if (CRC_8_TABLE_SIZE > 0U) /* CRC8 generation via table */
static const Crc_Table8[CRC_8_TABLE_SIZE];
#endif


Crc8::Crc8()
{
   // TODO Auto-generated constructor stub

}

Crc8::~Crc8()
{
   // TODO Auto-generated destructor stub
}


#if (CRC_8_ENABLED == STD_ON)

uint8_t Crc8::calculate(uint8* Crc_DataPtr, uint32 Crc_Length,
                        uint8 Crc_StartValue8, boolean Crc_IsFirstCall) {

   if (true == Crc_IsFirstCall) {
      Crc_StartValue8 = CRC8_INITIAL_VALUE;
   } else {
      /* undo the XOR on the incoming value */
      Crc_StartValue8 ^= 0xFFU;
   }

   /* Process all data (byte wise) */
   while (Crc_Length != 0U) {
#if (CRC_8_TABLE_SIZE == 16U) /* CRC8 generation with small table */

      /* Process high nibble of data byte */
      Crc_StartValue8
      = Crc_Table8[
      ((uint8)(Crc_StartValue8 >> 4U)) ^ ((uint8)(*Crc_DataPtr >> 4U))]
      ^ ((uint8)(Crc_StartValue8 << 4U));

      /* Process low nibble of data byte */
      Crc_StartValue8
      = Crc_Table8[
      ((uint8)(Crc_StartValue8 >> 4U)) ^ (*Crc_DataPtr & 0x0FU)]
      ^ ((uint8)(Crc_StartValue8 << 4U));

#elif (CRC_8_TABLE_SIZE == 256U) /* CRC8 generation with large table */

      Crc_StartValue8 = Crc_Table8[Crc_StartValue8 ^ *Crc_DataPtr];

#else /* CRC8 generation at runtime */

      uint8_t i; /* loop counter */

      Crc_StartValue8 ^= *Crc_DataPtr;

      /* calculate crc bit by bit */
      for (i = 0U; i < 8U; ++i) {
         /* if highest bit set to zero */
         if ((Crc_StartValue8 & 0x80U) == 0U) {
            /* no need to xor with the polynomial, just shift */
            Crc_StartValue8 <<= 1U;
         } else {
            /* bit was set to one: xor it with the CRC8 polynomial */
            Crc_StartValue8 = ((uint8) (Crc_StartValue8 << 1U))
                  ^ CRC8_POLYNOMIAL;
         }
      }

#endif

      /* Advance the pointer and decrease remaining bytes to calculate over
       * until all bytes in the buffer have been used as input */
      /* Deviation MISRA-1 */
      ++Crc_DataPtr;
      --Crc_Length;
   } /* while (Crc_Length != 0) */

   /* Note that the Autosar R3.1 CRC SWS specifies a xor value of 0 which is
    * wrong.  The Autosar R4.0 CRC SWS specifies the corrected xor value of
    * 0xFF. */
   Crc_StartValue8 ^= 0xFFU; /* XOR crc value */

   return Crc_StartValue8;
}

#endif
