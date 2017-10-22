/*
 * Crc8H2F.cpp
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#include "Crc8H2F.h"


#if (CRC_8H2F_TABLE_SIZE > 0U) /* CRC8H2F generation via table */

/* Table of pre-computed values for CRC8H2F */
static const Crc_Table8H2F[CRC_8H2F_TABLE_SIZE]
#endif


Crc8H2F::Crc8H2F()
{
   // TODO Auto-generated constructor stub

}

Crc8H2F::~Crc8H2F()
{
   // TODO Auto-generated destructor stub
}


#if (CRC_8H2F_ENABLED == STD_ON)

uint8_t Crc8H2F::calculate(uint8_t* Crc_DataPtr,
                           uint32_t Crc_Length,
                           uint8_t Crc_StartValue8H2F,
                           boolean Crc_IsFirstCall)
{

   uint8_t i; /* loop counter */

   if (true == Crc_IsFirstCall) {
      Crc_StartValue8H2F = CRC8H2F_INITIAL_VALUE;
   } else {
      /* undo the XOR on the incoming value */
      Crc_StartValue8H2F ^= 0xFFU;
   }

   /* Process all data (byte wise) */
   while (Crc_Length != 0U) {
#if (CRC_8H2F_TABLE_SIZE == 16U) /* CRC8H2F generation with small table */

      /* Process high nibble of data byte */
      Crc_StartValue8H2F
      = Crc_Table8H2F[
      ((uint8)(Crc_StartValue8H2F >> 4U)) ^ ((uint8)(*Crc_DataPtr >> 4U))]
      ^ ((uint8)(Crc_StartValue8H2F << 4U));

      /* Process low nibble of data byte */
      Crc_StartValue8H2F
      = Crc_Table8H2F[
      ((uint8)(Crc_StartValue8H2F >> 4U)) ^ (*Crc_DataPtr & 0x0FU)]
      ^ ((uint8)(Crc_StartValue8H2F << 4U));

#elif (CRC_8H2F_TABLE_SIZE == 256U) /* CRC8H2F generation with large table */

      Crc_StartValue8H2F = Crc_Table8H2F[Crc_StartValue8H2F ^ *Crc_DataPtr];

#else /* CRC8H2F generation at runtime */

      Crc_StartValue8H2F ^= *Crc_DataPtr;

      /* calculate crc bit by bit */
      for (i = 0U; i < 8U; ++i) {
         /* if highest bit set to zero */
         if ((Crc_StartValue8H2F & 0x80U) == 0U) {
            /* no need to xor with the polynomial, just shift */
            Crc_StartValue8H2F <<= 1U;
         } else {
            /* bit was set to one: xor it with the CRC8 polynomial */
            Crc_StartValue8H2F = ((uint8) (Crc_StartValue8H2F << 1U))
                  ^ CRC8H2F_POLYNOMIAL;
         }
      }

#endif

      /* Advance the pointer and decrease remaining bytes to calculate over
       * until all bytes in the buffer have been used as input */
      /* Deviation MISRA-1 */
      ++Crc_DataPtr;
      --Crc_Length;
   } /* while (Crc_Length != 0) */

   Crc_StartValue8H2F ^= 0xFFU; /* XOR crc value */

   return Crc_StartValue8H2F;
}

#endif


