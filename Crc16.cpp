/*
 * Crc16.cpp
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#include "Crc16.h"


#if (CRC_16_TABLE_SIZE > 0U) /* CRC16 generation via table */

/* Table of pre-computed values for CRC16. Used Polynomial is 0x1021 */
static const Crc_Table16[CRC_16_TABLE_SIZE];
#endif


Crc16::Crc16()
{
   // TODO Auto-generated constructor stub

}

Crc16::~Crc16()
{
   // TODO Auto-generated destructor stub
}

#if (CRC_16_ENABLED == STD_ON)

uint16_t Crc16::calculate(uint8_t* Crc_DataPtr,
                        uint32_t Crc_Length,
                        uint16_t Crc_StartValue16,
                        boolean Crc_IsFirstCall
                       )
{

   uint8_t i; /* loop counter */

   if (true == Crc_IsFirstCall)
   {
      Crc_StartValue16 = CRC16_INITIAL_VALUE;
   }

   /* Process all data (byte wise) */
   while (Crc_Length != 0U)
   {
#if (CRC_16_TABLE_SIZE == 16U) /* CRC16 generation with small table */

      /* Process high nibble of actual data */
      Crc_StartValue16
      = Crc_Table16[
      ((uint8)(Crc_StartValue16 >> 12U)) ^ ((uint8)(*Crc_DataPtr >> 4U))]
      ^ ((uint16)(Crc_StartValue16 << 4U));

      /* Process low nibble of actual data */
      Crc_StartValue16
      = Crc_Table16[
      ((uint8)(Crc_StartValue16 >> 12U)) ^ (*Crc_DataPtr & 0x0FU)]
      ^ ((uint16)(Crc_StartValue16 << 4U));

#elif (CRC_16_TABLE_SIZE == 256U) /* CRC16 generation with large table */

      /* Process one byte of data */
      Crc_StartValue16
      = Crc_Table16[((uint8)(Crc_StartValue16 >> 8U)) ^ *Crc_DataPtr]
      ^ ((uint16)(Crc_StartValue16 << 8U));

#else /* CRC16 generation at runtime */

      Crc_StartValue16 ^= (uint16)(((uint16)*Crc_DataPtr) << 8U);

      /* calculate crc bit by bit */
      for (i = 0U; i < 8U; ++i)
      {
         /* if highest bit set to zero */
         if ((Crc_StartValue16 & 0x8000U) == 0U)
         {
            /* no need to xor the zero bit with the polynomial, just shift */
            Crc_StartValue16 <<= 1U;
         }
         else
         {
            /* bit was set to one: xor it with the CRC16 polynomial */
            Crc_StartValue16
            = ((uint16)(Crc_StartValue16 << 1U)) ^ CRC16_POLYNOMIAL;
         }
      }

#endif

      /* Advance the pointer and decrease remaining bytes to calculate over
       * until all bytes in the buffer have been used as input */
      /* Deviation MISRA-1 */
      ++Crc_DataPtr;
      --Crc_Length;
   } /* while (Crc_Length != 0U) */

   /* specified final XOR value for CRC16 is 0, no need to actually xor
    * anything here */
   return Crc_StartValue16;
}
#endif


