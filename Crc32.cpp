/*
 * Crc32.cpp
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#include "Crc32.h"


#if (CRC_32_TABLE_SIZE > 0U) /* CRC32 generation via table */

/* Table of pre-computed reflected values for CRC32. Used Polynomial is
 * 0x04c11db7 */
static const Crc_Table32[CRC_32_TABLE_SIZE];
#endif

Crc32::Crc32()
{
   // TODO Auto-generated constructor stub

}

Crc32::~Crc32()
{
   // TODO Auto-generated destructor stub
}


#if (CRC_32_ENABLED == STD_ON)

uint32_t Crc32::calculate(uint8_t* Crc_DataPtr,
                        uint32_t Crc_Length,
                        uint32_t Crc_StartValue32,
                        boolean Crc_IsFirstCall
                       )
{

   uint8_t i; /* loop counter */

   if (true == Crc_IsFirstCall)
   {
      Crc_StartValue32 = CRC32_INITIAL_VALUE;
   }
   else
   {
      /* undo the XOR on the start value */
      Crc_StartValue32 ^= 0xFFFFFFFFU;

      /* The reflection of the initial value is not necessary here as we used
       * the "reflected" algorithm and reflected table values. */
   }

   /* Process all data byte-wise */
   while (Crc_Length != 0U)
   {
#if (CRC_32_TABLE_SIZE == 16U) /* CRC32 generation via small table */

      /* Process low nibble of actual data */
      Crc_StartValue32
      = Crc_Table32[0x0FU & (Crc_StartValue32 ^ *Crc_DataPtr)]
      ^ (Crc_StartValue32 >> 4U);

      /* Process high nibble of actual data */
      Crc_StartValue32
      = Crc_Table32[
      0x0FU & (Crc_StartValue32 ^ ((uint32)*Crc_DataPtr >> 4U))]
      ^ (Crc_StartValue32 >> 4U);

#elif (CRC_32_TABLE_SIZE == 256U) /* CRC32 generation via large table */

      /* Process one byte of data */
      Crc_StartValue32
      = Crc_Table32[((uint8)Crc_StartValue32) ^ *Crc_DataPtr]
      ^ (Crc_StartValue32 >> 8U);

#else /* CRC32 generation at runtime */

      Crc_StartValue32 ^= *Crc_DataPtr;

      /* calculate crc bit by bit */
      for (i = 0U; i < 8U; ++i)
      {
         /* Test value uf the lowest bit.  Note that the CRC32 works on
          * reflected data in contrast to CRC8 and CRC16 and does therfore
          * start with the least significant bit. */
         if ((Crc_StartValue32 & 1U) == 0U)
         {
            /* no need to xor with the polynomial, just shift */
            Crc_StartValue32 >>= 1U;
         }
         else
         {
            /* bit was set to one: xor it with the reflected CRC32
             * polynomial */
            Crc_StartValue32 = (Crc_StartValue32 >> 1U) ^ CRC32_POLYNOMIAL;
         }
      }

#endif

      /* Advance the pointer and decrease remaining bytes to calculate over
       * until all bytes in the buffer have been used as input */
      /* Deviation MISRA-1 */
      ++Crc_DataPtr;
      --Crc_Length;
   } /* while (Crc_Length != 0U) */

   /* The reflection of the remainder is not necessary here as we used the
    * "reflected" algorithm and reflected table values. */

   Crc_StartValue32 ^= 0xFFFFFFFFU; /* XOR crc value */

   return Crc_StartValue32;
}

#endif
