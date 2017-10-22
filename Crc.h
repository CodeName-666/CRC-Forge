
#ifndef CRC_H
#define CRC_H


#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#if (CRC_8_ENABLED == STD_ON)

/** @brief Calculation of CRC8
 *
 * This function performs the calculation of a 8-bit SAE J1850 CRC value
 * (runtime or table variant, depending on Configuration Parameter Crc8Mode)
 * over the memory block referenced by \p Crc_DataPtr of byte length \p
 * Crc_Length.
 *
 * @param[in] Crc_DataPtr Valid pointer to start address of data block
 * @param[in] Crc_Length  Length of data block in bytes
 * @param[in] Crc_StartValue8 Initial Value
 * @param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 * CRC calculation; start from initial value, ignore Crc_StartValue8. FALSE:
 * Subsequent call in a call sequence; Crc_StartValue8 is interpreted to be
 * the return value of the previous function call.
 * @return 8 bit result of CRC calculation.
 *
 */
extern uint8 Crc_CalculateCRC8(uint8* Crc_DataPtr,
                               uint32  Crc_Length,
                               uint8   Crc_StartValue8,
                               boolean Crc_IsFirstCall
                              );

#endif

#if (CRC_8H2F_ENABLED == STD_ON)

/** @brief Calculation of CRC8H2F
 **
 ** This function performs the calculation of a 8-bit CRC with polynom 0x2F
 ** (runtime or table variant, depending on Configuration Parameter
 ** Crc8H2FMode) over the memory block referenced by \p Crc_DataPtr of byte
 ** length \p Crc_Length.
 **
 ** @param[in] Crc_DataPtr Valid pointer to start address of data block
 ** @param[in] Crc_Length  Length of data block in bytes
 ** @param[in] Crc_StartValue8H2F Initial Value
 ** @param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore \p
 ** Crc_StartValue8H2F. FALSE: Subsequent call in a call sequence; \p
 ** Crc_StartValue8H2F is interpreted to be the return value of the previous
 ** function call.
 ** @return 8 bit result of CRC calculation.
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern uint8 Crc_CalculateCRC8H2F(uint8* Crc_DataPtr,
                                  uint32  Crc_Length,
                                  uint8   Crc_StartValue8H2F,
                                  boolean Crc_IsFirstCall
                                 );

#endif

#if (CRC_16_ENABLED == STD_ON)

/** @brief Calculation of CRC16
 *
 * This function performs the calculation of a CRC16 value (runtime or table
 * variant, depending on Configuration Parameter Crc16Mode) over the memory
 * block referenced by \p Crc_DataPtr of byte length \p Crc_Length.
 *
 * @param[in] Crc_DataPtr Valid pointer to start address of data block
 * @param[in] Crc_Length  Length of data block in bytes
 * @param[in] Crc_StartValue16  Initial Value
 * @param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 * CRC calculation; start from initial value, ignore \p
 * Crc_StartValue16. FALSE: Subsequent call in a call sequence; \p
 * Crc_StartValue16 is interpreted to be the return value of the previous
 * function call.
 * @return 16 bit result of CRC calculation
 *
 */
extern uint16 Crc_CalculateCRC16(uint8* Crc_DataPtr,
                                 uint32  Crc_Length,
                                 uint16  Crc_StartValue16,
                                 boolean Crc_IsFirstCall
                                );

#endif

#if (CRC_32_ENABLED == STD_ON)

/** @brief Calculation of CRC32
 **
 ** This function performs the calculation of a CRC32 value (runtime or table
 ** variant, depending on Configuration Parameter Crc32Mode) over the memory
 ** block referenced by \p Crc_DataPtr of byte length \p Crc_Length.
 **
 ** @param[in] Crc_DataPtr Valid pointer to start address of data block
 ** @param[in] Crc_Length  Length of data block in bytes
 ** @param[in] Crc_StartValue32  Initial Value
 ** @param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore Crc_StartValue32. FALSE:
 ** Subsequent call in a call sequence; Crc_StartValue32 is interpreted to be
 ** the return value of the previous function call.
 ** @return calculated CRC32 value
 **
 ** \ServiceID{3}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern uint32 Crc_CalculateCRC32(uint8* Crc_DataPtr,
                                 uint32  Crc_Length,
                                 uint32  Crc_StartValue32,
                                 boolean Crc_IsFirstCall
                                );

#endif

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* if !defined( CRC_H ) */
/*==================[end of file]============================================*/
