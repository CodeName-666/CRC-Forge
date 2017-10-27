/*
 * Crc16.cpp
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#include "Crc16.h"


#if (CRC16_TABLE_SIZE > 0U) /* CRC16 generation via table */

/* Table of pre-computed values for CRC16. Used Polynomial is 0x1021 */
extern const uint16_t Crc_Table16[CRC16_TABLE_SIZE];
#endif




Crc16::Crc16()
{
   // TODO Auto-generated constructor stub

}

Crc16::~Crc16()
{
   // TODO Auto-generated destructor stub
}


uint16_t Crc16::calculate(uint8_t* dataPtr,
                        uint32_t dataLength,
                        uint16_t startValue,
                        boolean isFirstCall
                       )
{
#if (CRC16_TABLE_SIZE == 0U)
   uint8_t i; /* loop counter */
#endif

   if (true == isFirstCall)
   {
      startValue = CRC16_INITIAL_VALUE;
   }

   /* Process all data (byte wise) */
   while (dataLength != 0U)
   {
#if (CRC16_TABLE_SIZE == 16U) /* CRC16 generation with small table */

      /* Process high nibble of actual data */
      startValue
      = Crc_Table16[
      ((uint8)(startValue >> 12U)) ^ ((uint8)(*dataPtr >> 4U))]
      ^ ((uint16)(startValue << 4U));

      /* Process low nibble of actual data */
      startValue
      = Crc_Table16[
      ((uint8)(startValue >> 12U)) ^ (*dataPtr & 0x0FU)]
      ^ ((uint16)(startValue << 4U));

#elif (CRC16_TABLE_SIZE == 256U) /* CRC16 generation with large table */

      /* Process one byte of data */
      startValue
      = Crc_Table16[((uint8)(startValue >> 8U)) ^ *dataPtr]
      ^ ((uint16)(startValue << 8U));

#else /* CRC16 generation at runtime */

      startValue ^= (uint16)(((uint16)*dataPtr) << 8U);

      /* calculate crc bit by bit */
      for (i = 0U; i < 8U; ++i)
      {
         /* if highest bit set to zero */
         if ((startValue & 0x8000U) == 0U)
         {
            /* no need to xor the zero bit with the polynomial, just shift */
            startValue <<= 1U;
         }
         else
         {
            /* bit was set to one: xor it with the CRC16 polynomial */
            startValue
            = ((uint16)(startValue << 1U)) ^ CRC16_POLYNOMIAL;
         }
      }

#endif

      /* Advance the pointer and decrease remaining bytes to calculate over
       * until all bytes in the buffer have been used as input */
      /* Deviation MISRA-1 */
      ++dataPtr;
      --dataLength;
   } /* while (Crc_Length != 0U) */

   /* specified final XOR value for CRC16 is 0, no need to actually xor
    * anything here */
   return startValue;
}



#if (CRC16_TABLE_SIZE > 0U) /* CRC16 generation via table */

/* Table of pre-computed values for CRC16. Used Polynomial is 0x1021 */
const uint16_t Crc_Table16[CRC16_TABLE_SIZE] =
{
   0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
   0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
#if (CRC16_TABLE_SIZE == 256U)
   0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
   0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
   0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
   0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
   0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
   0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
   0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
   0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
   0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
   0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
   0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
   0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
   0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
   0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
   0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
   0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
   0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
   0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
   0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
   0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
   0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
   0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
   0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
   0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
   0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
   0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
   0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
   0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
   0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
   0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U
#endif
};


#endif


