/**
 * \file
 *
 * \brief AUTOSAR Crc
 *
 * This file contains the implementation of the AUTOSAR
 * module Crc.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2017 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 17.4 (required)
 *    "Array indexing shall be the only allowed form of pointer arithmetic."
 *
 *    Reason: runtime optimization
 */

/* !LINKSTO CRC024,1 */

/*==================[inclusions]============================================*/

#include <Crc_Trace.h>
#include <Std_Types.h>         /* AUTOSAR standard types */

/* !LINKSTO CRC022,1 */
#include <Crc.h>               /* Module public API */

#if ( CRC_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO EB_CRC_DetHeaderFile,1 */
#include <Det.h>               /* API of module Det. */
#endif
/*==================[macros]================================================*/

/* !LINKSTO CRC005,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#if (!defined CRC_VENDOR_ID) /* configuration check */
#error CRC_VENDOR_ID must be defined
#endif

#if (CRC_VENDOR_ID != 1U) /* vendor check */
#error CRC_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#if (!defined CRC_AR_RELEASE_MAJOR_VERSION) /* configuration check */
#error CRC_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CRC_AR_RELEASE_MAJOR_VERSION != 4U)
#error CRC_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#if (!defined CRC_AR_RELEASE_MINOR_VERSION) /* configuration check */
#error CRC_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CRC_AR_RELEASE_MINOR_VERSION != 0U )
#error CRC_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#if (!defined CRC_AR_RELEASE_REVISION_VERSION) /* configuration check */
#error CRC_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (CRC_AR_RELEASE_REVISION_VERSION != 3U )
#error CRC_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#if (!defined CRC_SW_MAJOR_VERSION) /* configuration check */
#error CRC_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CRC_SW_MAJOR_VERSION != 6U)
#error CRC_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#if (!defined CRC_SW_MINOR_VERSION) /* configuration check */
#error CRC_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CRC_SW_MINOR_VERSION < 10U)
#error CRC_SW_MINOR_VERSION wrong (< 10U)
#endif

#if (!defined CRC_SW_PATCH_VERSION) /* configuration check */
#error CRC_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (CRC_SW_PATCH_VERSION < 1U)
#error CRC_SW_PATCH_VERSION wrong (< 1U)
#endif

/*------------------[CRC implementation macros]-----------------------------*/

/** \brief SAE J1850 CRC8 polynomial
 *
 * According to AUTOSAR R4.0 CRC SWS CRC030 */
#define CRC_POLYNOMIAL8     0x1DU

/** \brief CRC8 0x2F polynomial */
#define CRC_POLYNOMIAL8H2F  0x2FU

/** \brief definition of key width CRC16 polynomial [CRC002] */
#define CRC_POLYNOMIAL16    0x1021U

/** \brief definition of key width CRC32 polynomial [CRC002]
 **
 ** The CRC32 routine is based on IEEE-802.3 CRC32 Ethernet standard.
 ** In there, the polynomial 0x04C11DB7 is specified to be used.
 **
 ** In that standard, the reflection of all input bytes is specified.
 ** We use an optimized algorithm where we do not reflect the input
 ** but the polynomial.
 ** So the polynomial 0xEDB88320 specified below is the reflected
 ** polynomial of the polynomial 0x04C11DB7.
 **
 ** See the "A Painless Guide to CRC Error Detection Algorithms",
 ** R. Williams, 1993. */
#define CRC_POLYNOMIAL32    0xEDB88320U

#if (CRC_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to Det
 **
 ** \param[in] CRC_SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module
 **/
#define CRC_DET_REPORT_ERROR(CRC_SID,ERROR_CODE) \
  (void)Det_ReportError(CRC_MODULE_ID, CRC_INSTANCE_ID, (CRC_SID), (ERROR_CODE))


#endif /* if (CRC_DEV_ERROR_DETECT == STD_ON) */

/*------------------[CRC initial value]-------------------------------------*/

/** \brief Definition of the initial value of the SAE J1850 CRC8 */
#define CRC_INITIAL_VALUE8    0xFFU

/** \brief Definition of the initial value of the CRC8 on polynom 0x2F */
#define CRC_INITIAL_VALUE8H2F 0xFFU

/** \brief Definition of the initial value of crc16 */
#define CRC_INITIAL_VALUE16   0xFFFFU

/** \brief Definition of the initial value of crc32 */
#define CRC_INITIAL_VALUE32   0xFFFFFFFFU

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*------------------[version constants definition]--------------------------*/

/*==================[internal constants]====================================*/

#if (CRC_8_TABLE_SIZE > 0U) /* CRC8 generation via table */

#define CRC_START_SEC_CONST_8
#include <MemMap.h>

/* Table of pre-computed values for CRC8 */
STATIC CONST(uint8, CRC_CONST) Crc_Table8[CRC_8_TABLE_SIZE] =
{
  0x00U, 0x1DU, 0x3AU, 0x27U, 0x74U, 0x69U, 0x4EU, 0x53U, 0xE8U, 0xF5U, 0xD2U,
  0xCFU, 0x9CU, 0x81U, 0xA6U, 0xBBU,
#if (CRC_8_TABLE_SIZE == 256U)
  0xCDU, 0xD0U, 0xF7U, 0xEAU, 0xB9U, 0xA4U, 0x83U, 0x9EU, 0x25U, 0x38U, 0x1FU,
  0x02U, 0x51U, 0x4CU, 0x6BU, 0x76U, 0x87U, 0x9AU, 0xBDU, 0xA0U, 0xF3U, 0xEEU,
  0xC9U, 0xD4U, 0x6FU, 0x72U, 0x55U, 0x48U, 0x1BU, 0x06U, 0x21U, 0x3CU, 0x4AU,
  0x57U, 0x70U, 0x6DU, 0x3EU, 0x23U, 0x04U, 0x19U, 0xA2U, 0xBFU, 0x98U, 0x85U,
  0xD6U, 0xCBU, 0xECU, 0xF1U, 0x13U, 0x0EU, 0x29U, 0x34U, 0x67U, 0x7AU, 0x5DU,
  0x40U, 0xFBU, 0xE6U, 0xC1U, 0xDCU, 0x8FU, 0x92U, 0xB5U, 0xA8U, 0xDEU, 0xC3U,
  0xE4U, 0xF9U, 0xAAU, 0xB7U, 0x90U, 0x8DU, 0x36U, 0x2BU, 0x0CU, 0x11U, 0x42U,
  0x5FU, 0x78U, 0x65U, 0x94U, 0x89U, 0xAEU, 0xB3U, 0xE0U, 0xFDU, 0xDAU, 0xC7U,
  0x7CU, 0x61U, 0x46U, 0x5BU, 0x08U, 0x15U, 0x32U, 0x2FU, 0x59U, 0x44U, 0x63U,
  0x7EU, 0x2DU, 0x30U, 0x17U, 0x0AU, 0xB1U, 0xACU, 0x8BU, 0x96U, 0xC5U, 0xD8U,
  0xFFU, 0xE2U, 0x26U, 0x3BU, 0x1CU, 0x01U, 0x52U, 0x4FU, 0x68U, 0x75U, 0xCEU,
  0xD3U, 0xF4U, 0xE9U, 0xBAU, 0xA7U, 0x80U, 0x9DU, 0xEBU, 0xF6U, 0xD1U, 0xCCU,
  0x9FU, 0x82U, 0xA5U, 0xB8U, 0x03U, 0x1EU, 0x39U, 0x24U, 0x77U, 0x6AU, 0x4DU,
  0x50U, 0xA1U, 0xBCU, 0x9BU, 0x86U, 0xD5U, 0xC8U, 0xEFU, 0xF2U, 0x49U, 0x54U,
  0x73U, 0x6EU, 0x3DU, 0x20U, 0x07U, 0x1AU, 0x6CU, 0x71U, 0x56U, 0x4BU, 0x18U,
  0x05U, 0x22U, 0x3FU, 0x84U, 0x99U, 0xBEU, 0xA3U, 0xF0U, 0xEDU, 0xCAU, 0xD7U,
  0x35U, 0x28U, 0x0FU, 0x12U, 0x41U, 0x5CU, 0x7BU, 0x66U, 0xDDU, 0xC0U, 0xE7U,
  0xFAU, 0xA9U, 0xB4U, 0x93U, 0x8EU, 0xF8U, 0xE5U, 0xC2U, 0xDFU, 0x8CU, 0x91U,
  0xB6U, 0xABU, 0x10U, 0x0DU, 0x2AU, 0x37U, 0x64U, 0x79U, 0x5EU, 0x43U, 0xB2U,
  0xAFU, 0x88U, 0x95U, 0xC6U, 0xDBU, 0xFCU, 0xE1U, 0x5AU, 0x47U, 0x60U, 0x7DU,
  0x2EU, 0x33U, 0x14U, 0x09U, 0x7FU, 0x62U, 0x45U, 0x58U, 0x0BU, 0x16U, 0x31U,
  0x2CU, 0x97U, 0x8AU, 0xADU, 0xB0U, 0xE3U, 0xFEU, 0xD9U, 0xC4U
#endif
};

#define CRC_STOP_SEC_CONST_8
#include <MemMap.h>

#endif  /* CRC_8_TABLE_SIZE > 0U */

#if (CRC_8H2F_TABLE_SIZE > 0U) /* CRC8H2F generation via table */

#define CRC_START_SEC_CONST_8
#include <MemMap.h>

/* Table of pre-computed values for CRC8H2F */
STATIC CONST(uint8, CRC_CONST) Crc_Table8H2F[CRC_8H2F_TABLE_SIZE] =
{
  0x00U, 0x2FU, 0x5EU, 0x71U, 0xBCU, 0x93U, 0xE2U, 0xCDU, 0x57U, 0x78U, 0x09U,
  0x26U, 0xEBU, 0xC4U, 0xB5U, 0x9AU,
#if (CRC_8_TABLE_SIZE == 256U)
  0xAEU, 0x81U, 0xF0U, 0xDFU, 0x12U, 0x3DU, 0x4CU, 0x63U, 0xF9U, 0xD6U, 0xA7U,
  0x88U, 0x45U, 0x6AU, 0x1BU, 0x34U, 0x73U, 0x5CU, 0x2DU, 0x02U, 0xCFU, 0xE0U,
  0x91U, 0xBEU, 0x24U, 0x0BU, 0x7AU, 0x55U, 0x98U, 0xB7U, 0xC6U, 0xE9U, 0xDDU,
  0xF2U, 0x83U, 0xACU, 0x61U, 0x4EU, 0x3FU, 0x10U, 0x8AU, 0xA5U, 0xD4U, 0xFBU,
  0x36U, 0x19U, 0x68U, 0x47U, 0xE6U, 0xC9U, 0xB8U, 0x97U, 0x5AU, 0x75U, 0x04U,
  0x2BU, 0xB1U, 0x9EU, 0xEFU, 0xC0U, 0x0DU, 0x22U, 0x53U, 0x7CU, 0x48U, 0x67U,
  0x16U, 0x39U, 0xF4U, 0xDBU, 0xAAU, 0x85U, 0x1FU, 0x30U, 0x41U, 0x6EU, 0xA3U,
  0x8CU, 0xFDU, 0xD2U, 0x95U, 0xBAU, 0xCBU, 0xE4U, 0x29U, 0x06U, 0x77U, 0x58U,
  0xC2U, 0xEDU, 0x9CU, 0xB3U, 0x7EU, 0x51U, 0x20U, 0x0FU, 0x3BU, 0x14U, 0x65U,
  0x4AU, 0x87U, 0xA8U, 0xD9U, 0xF6U, 0x6CU, 0x43U, 0x32U, 0x1DU, 0xD0U, 0xFFU,
  0x8EU, 0xA1U, 0xE3U, 0xCCU, 0xBDU, 0x92U, 0x5FU, 0x70U, 0x01U, 0x2EU, 0xB4U,
  0x9BU, 0xEAU, 0xC5U, 0x08U, 0x27U, 0x56U, 0x79U, 0x4DU, 0x62U, 0x13U, 0x3CU,
  0xF1U, 0xDEU, 0xAFU, 0x80U, 0x1AU, 0x35U, 0x44U, 0x6BU, 0xA6U, 0x89U, 0xF8U,
  0xD7U, 0x90U, 0xBFU, 0xCEU, 0xE1U, 0x2CU, 0x03U, 0x72U, 0x5DU, 0xC7U, 0xE8U,
  0x99U, 0xB6U, 0x7BU, 0x54U, 0x25U, 0x0AU, 0x3EU, 0x11U, 0x60U, 0x4FU, 0x82U,
  0xADU, 0xDCU, 0xF3U, 0x69U, 0x46U, 0x37U, 0x18U, 0xD5U, 0xFAU, 0x8BU, 0xA4U,
  0x05U, 0x2AU, 0x5BU, 0x74U, 0xB9U, 0x96U, 0xE7U, 0xC8U, 0x52U, 0x7DU, 0x0CU,
  0x23U, 0xEEU, 0xC1U, 0xB0U, 0x9FU, 0xABU, 0x84U, 0xF5U, 0xDAU, 0x17U, 0x38U,
  0x49U, 0x66U, 0xFCU, 0xD3U, 0xA2U, 0x8DU, 0x40U, 0x6FU, 0x1EU, 0x31U, 0x76U,
  0x59U, 0x28U, 0x07U, 0xCAU, 0xE5U, 0x94U, 0xBBU, 0x21U, 0x0EU, 0x7FU, 0x50U,
  0x9DU, 0xB2U, 0xC3U, 0xECU, 0xD8U, 0xF7U, 0x86U, 0xA9U, 0x64U, 0x4BU, 0x3AU,
  0x15U, 0x8FU, 0xA0U, 0xD1U, 0xFEU, 0x33U, 0x1CU, 0x6DU, 0x42U
#endif
};

#define CRC_STOP_SEC_CONST_8
#include <MemMap.h>

#endif  /* CRC_8H2F_TABLE_SIZE > 0U */

#if (CRC_16_TABLE_SIZE > 0U) /* CRC16 generation via table */

#define CRC_START_SEC_CONST_16
#include <MemMap.h>

/* Table of pre-computed values for CRC16. Used Polynomial is 0x1021 */
STATIC CONST(uint16, CRC_CONST) Crc_Table16[CRC_16_TABLE_SIZE] =
{
  0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
  0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
#if (CRC_16_TABLE_SIZE == 256U)
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

#define CRC_STOP_SEC_CONST_16
#include <MemMap.h>

#endif /* CRC_16_TABLE_SIZE > 0U */

#if (CRC_32_TABLE_SIZE > 0U) /* CRC32 generation via table */

#define CRC_START_SEC_CONST_32
#include <MemMap.h>

/* Table of pre-computed reflected values for CRC32. Used Polynomial is
 * 0x04c11db7 */
STATIC CONST(uint32, CRC_CONST) Crc_Table32[CRC_32_TABLE_SIZE] =
{
#if (CRC_32_TABLE_SIZE == 16U)
  0x00000000U, 0x1DB71064U, 0x3B6E20C8U, 0x26D930ACU, 0x76DC4190U,
  0x6B6B51F4U, 0x4DB26158U, 0x5005713CU, 0xEDB88320U, 0xF00F9344U,
  0xD6D6A3E8U, 0xCB61B38CU, 0x9B64C2B0U, 0x86D3D2D4U, 0xA00AE278U,
  0xBDBDF21CU
#else
  0x00000000U, 0x77073096U, 0xEE0E612CU, 0x990951BAU, 0x076DC419U,
  0x706AF48FU, 0xE963A535U, 0x9E6495A3U, 0x0EDB8832U, 0x79DCB8A4U,
  0xE0D5E91EU, 0x97D2D988U, 0x09B64C2BU, 0x7EB17CBDU, 0xE7B82D07U,
  0x90BF1D91U, 0x1DB71064U, 0x6AB020F2U, 0xF3B97148U, 0x84BE41DEU,
  0x1ADAD47DU, 0x6DDDE4EBU, 0xF4D4B551U, 0x83D385C7U, 0x136C9856U,
  0x646BA8C0U, 0xFD62F97AU, 0x8A65C9ECU, 0x14015C4FU, 0x63066CD9U,
  0xFA0F3D63U, 0x8D080DF5U, 0x3B6E20C8U, 0x4C69105EU, 0xD56041E4U,
  0xA2677172U, 0x3C03E4D1U, 0x4B04D447U, 0xD20D85FDU, 0xA50AB56BU,
  0x35B5A8FAU, 0x42B2986CU, 0xDBBBC9D6U, 0xACBCF940U, 0x32D86CE3U,
  0x45DF5C75U, 0xDCD60DCFU, 0xABD13D59U, 0x26D930ACU, 0x51DE003AU,
  0xC8D75180U, 0xBFD06116U, 0x21B4F4B5U, 0x56B3C423U, 0xCFBA9599U,
  0xB8BDA50FU, 0x2802B89EU, 0x5F058808U, 0xC60CD9B2U, 0xB10BE924U,
  0x2F6F7C87U, 0x58684C11U, 0xC1611DABU, 0xB6662D3DU, 0x76DC4190U,
  0x01DB7106U, 0x98D220BCU, 0xEFD5102AU, 0x71B18589U, 0x06B6B51FU,
  0x9FBFE4A5U, 0xE8B8D433U, 0x7807C9A2U, 0x0F00F934U, 0x9609A88EU,
  0xE10E9818U, 0x7F6A0DBBU, 0x086D3D2DU, 0x91646C97U, 0xE6635C01U,
  0x6B6B51F4U, 0x1C6C6162U, 0x856530D8U, 0xF262004EU, 0x6C0695EDU,
  0x1B01A57BU, 0x8208F4C1U, 0xF50FC457U, 0x65B0D9C6U, 0x12B7E950U,
  0x8BBEB8EAU, 0xFCB9887CU, 0x62DD1DDFU, 0x15DA2D49U, 0x8CD37CF3U,
  0xFBD44C65U, 0x4DB26158U, 0x3AB551CEU, 0xA3BC0074U, 0xD4BB30E2U,
  0x4ADFA541U, 0x3DD895D7U, 0xA4D1C46DU, 0xD3D6F4FBU, 0x4369E96AU,
  0x346ED9FCU, 0xAD678846U, 0xDA60B8D0U, 0x44042D73U, 0x33031DE5U,
  0xAA0A4C5FU, 0xDD0D7CC9U, 0x5005713CU, 0x270241AAU, 0xBE0B1010U,
  0xC90C2086U, 0x5768B525U, 0x206F85B3U, 0xB966D409U, 0xCE61E49FU,
  0x5EDEF90EU, 0x29D9C998U, 0xB0D09822U, 0xC7D7A8B4U, 0x59B33D17U,
  0x2EB40D81U, 0xB7BD5C3BU, 0xC0BA6CADU, 0xEDB88320U, 0x9ABFB3B6U,
  0x03B6E20CU, 0x74B1D29AU, 0xEAD54739U, 0x9DD277AFU, 0x04DB2615U,
  0x73DC1683U, 0xE3630B12U, 0x94643B84U, 0x0D6D6A3EU, 0x7A6A5AA8U,
  0xE40ECF0BU, 0x9309FF9DU, 0x0A00AE27U, 0x7D079EB1U, 0xF00F9344U,
  0x8708A3D2U, 0x1E01F268U, 0x6906C2FEU, 0xF762575DU, 0x806567CBU,
  0x196C3671U, 0x6E6B06E7U, 0xFED41B76U, 0x89D32BE0U, 0x10DA7A5AU,
  0x67DD4ACCU, 0xF9B9DF6FU, 0x8EBEEFF9U, 0x17B7BE43U, 0x60B08ED5U,
  0xD6D6A3E8U, 0xA1D1937EU, 0x38D8C2C4U, 0x4FDFF252U, 0xD1BB67F1U,
  0xA6BC5767U, 0x3FB506DDU, 0x48B2364BU, 0xD80D2BDAU, 0xAF0A1B4CU,
  0x36034AF6U, 0x41047A60U, 0xDF60EFC3U, 0xA867DF55U, 0x316E8EEFU,
  0x4669BE79U, 0xCB61B38CU, 0xBC66831AU, 0x256FD2A0U, 0x5268E236U,
  0xCC0C7795U, 0xBB0B4703U, 0x220216B9U, 0x5505262FU, 0xC5BA3BBEU,
  0xB2BD0B28U, 0x2BB45A92U, 0x5CB36A04U, 0xC2D7FFA7U, 0xB5D0CF31U,
  0x2CD99E8BU, 0x5BDEAE1DU, 0x9B64C2B0U, 0xEC63F226U, 0x756AA39CU,
  0x026D930AU, 0x9C0906A9U, 0xEB0E363FU, 0x72076785U, 0x05005713U,
  0x95BF4A82U, 0xE2B87A14U, 0x7BB12BAEU, 0x0CB61B38U, 0x92D28E9BU,
  0xE5D5BE0DU, 0x7CDCEFB7U, 0x0BDBDF21U, 0x86D3D2D4U, 0xF1D4E242U,
  0x68DDB3F8U, 0x1FDA836EU, 0x81BE16CDU, 0xF6B9265BU, 0x6FB077E1U,
  0x18B74777U, 0x88085AE6U, 0xFF0F6A70U, 0x66063BCAU, 0x11010B5CU,
  0x8F659EFFU, 0xF862AE69U, 0x616BFFD3U, 0x166CCF45U, 0xA00AE278U,
  0xD70DD2EEU, 0x4E048354U, 0x3903B3C2U, 0xA7672661U, 0xD06016F7U,
  0x4969474DU, 0x3E6E77DBU, 0xAED16A4AU, 0xD9D65ADCU, 0x40DF0B66U,
  0x37D83BF0U, 0xA9BCAE53U, 0xDEBB9EC5U, 0x47B2CF7FU, 0x30B5FFE9U,
  0xBDBDF21CU, 0xCABAC28AU, 0x53B39330U, 0x24B4A3A6U, 0xBAD03605U,
  0xCDD70693U, 0x54DE5729U, 0x23D967BFU, 0xB3667A2EU, 0xC4614AB8U,
  0x5D681B02U, 0x2A6F2B94U, 0xB40BBE37U, 0xC30C8EA1U, 0x5A05DF1BU,
  0x2D02EF8DU
#endif
};

#define CRC_STOP_SEC_CONST_32
#include <MemMap.h>

#endif /* CRC_32_TABLE_SIZE > 0U */

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define CRC_START_SEC_CODE
#include <MemMap.h>



FUNC(void, CRC_CODE) Crc_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRC_APPL_DATA) VersionInfoPtr
)
{
  DBG_CRC_GETVERSIONINFO_ENTRY(VersionInfoPtr);
#if ( CRC_DEV_ERROR_DETECT == STD_ON )
  if(NULL_PTR == VersionInfoPtr)
  {
    CRC_DET_REPORT_ERROR( CRC_GET_VERSION_INFO_API_ID, CRC_E_PARAM_DATA );
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = CRC_VENDOR_ID;
    VersionInfoPtr->moduleID         = CRC_MODULE_ID;
    VersionInfoPtr->sw_major_version = CRC_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = CRC_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = CRC_SW_PATCH_VERSION;
  }
  DBG_CRC_GETVERSIONINFO_EXIT(VersionInfoPtr);
}


#if (CRC_8_ENABLED == STD_ON)

FUNC(uint8, CRC_CODE) Crc_CalculateCRC8
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint8                                    Crc_StartValue8,
  boolean                                  Crc_IsFirstCall
)
{

  DBG_CRC_CALCULATECRC8_ENTRY(Crc_DataPtr,Crc_Length,Crc_StartValue8,Crc_IsFirstCall);

  if (TRUE == Crc_IsFirstCall)
  {
    Crc_StartValue8 = CRC_INITIAL_VALUE8;
  }
  else
  {
    /* undo the XOR on the incoming value */
    Crc_StartValue8 ^= 0xFFU;
  }

  /* Process all data (byte wise) */
  while (Crc_Length != 0U)
  {
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

    uint8_least i;            /* loop counter */

    Crc_StartValue8 ^= *Crc_DataPtr;

    /* calculate crc bit by bit */
    for (i = 0U; i < 8U; ++i)
    {
      /* if highest bit set to zero */
      if ((Crc_StartValue8 & 0x80U) == 0U)
      {
        /* no need to xor with the polynomial, just shift */
        Crc_StartValue8 <<= 1U;
      }
      else
      {
        /* bit was set to one: xor it with the CRC8 polynomial */
        Crc_StartValue8 = ((uint8)(Crc_StartValue8 << 1U)) ^ CRC_POLYNOMIAL8;
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
  Crc_StartValue8 ^= 0xFFU;   /* XOR crc value */

  DBG_CRC_CALCULATECRC8_EXIT(Crc_StartValue8,Crc_DataPtr,Crc_Length,Crc_StartValue8,Crc_IsFirstCall);
  return Crc_StartValue8;
}

#endif

#if (CRC_8H2F_ENABLED == STD_ON)

FUNC(uint8, CRC_CODE) Crc_CalculateCRC8H2F
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint8                                    Crc_StartValue8H2F,
  boolean                                  Crc_IsFirstCall
)
{

  DBG_CRC_CALCULATECRC8H2F_ENTRY(Crc_DataPtr,Crc_Length,Crc_StartValue8H2F,Crc_IsFirstCall);

  if (TRUE == Crc_IsFirstCall)
  {
    Crc_StartValue8H2F = CRC_INITIAL_VALUE8H2F;
  }
  else
  {
    /* undo the XOR on the incoming value */
    Crc_StartValue8H2F ^= 0xFFU;
  }

  /* Process all data (byte wise) */
  while (Crc_Length != 0U)
  {
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

    uint8_least i;            /* loop counter */

    Crc_StartValue8H2F ^= *Crc_DataPtr;

    /* calculate crc bit by bit */
    for (i = 0U; i < 8U; ++i)
    {
      /* if highest bit set to zero */
      if ((Crc_StartValue8H2F & 0x80U) == 0U)
      {
        /* no need to xor with the polynomial, just shift */
        Crc_StartValue8H2F <<= 1U;
      }
      else
      {
        /* bit was set to one: xor it with the CRC8 polynomial */
        Crc_StartValue8H2F
          = ((uint8)(Crc_StartValue8H2F << 1U)) ^ CRC_POLYNOMIAL8H2F;
      }
    }

#endif

    /* Advance the pointer and decrease remaining bytes to calculate over
     * until all bytes in the buffer have been used as input */
    /* Deviation MISRA-1 */
    ++Crc_DataPtr;
    --Crc_Length;
  } /* while (Crc_Length != 0) */

  Crc_StartValue8H2F ^= 0xFFU;   /* XOR crc value */

  DBG_CRC_CALCULATECRC8H2F_EXIT(Crc_StartValue8H2F,Crc_DataPtr,Crc_Length,Crc_StartValue8H2F,Crc_IsFirstCall);
  return Crc_StartValue8H2F;
}

#endif

#if (CRC_16_ENABLED == STD_ON)

FUNC(uint16, CRC_CODE) Crc_CalculateCRC16
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint16                                   Crc_StartValue16,
  boolean                                  Crc_IsFirstCall
)
{

  DBG_CRC_CALCULATECRC16_ENTRY(Crc_DataPtr,Crc_Length,Crc_StartValue16,Crc_IsFirstCall);

  if (TRUE == Crc_IsFirstCall)
  {
    Crc_StartValue16 = CRC_INITIAL_VALUE16;
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

    uint8_least i;            /* loop counter */

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
          = ((uint16)(Crc_StartValue16 << 1U)) ^ CRC_POLYNOMIAL16;
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

  DBG_CRC_CALCULATECRC16_EXIT(Crc_StartValue16,Crc_DataPtr,Crc_Length,Crc_StartValue16,Crc_IsFirstCall);
  return Crc_StartValue16;
}

#endif

#if (CRC_32_ENABLED == STD_ON)

FUNC(uint32, CRC_CODE) Crc_CalculateCRC32
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint32                                   Crc_StartValue32,
  boolean                                  Crc_IsFirstCall
)
{

  DBG_CRC_CALCULATECRC32_ENTRY(Crc_DataPtr,Crc_Length,Crc_StartValue32,Crc_IsFirstCall);

  if (TRUE == Crc_IsFirstCall)
  {
    Crc_StartValue32 = CRC_INITIAL_VALUE32;
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

    uint8_least i;            /* loop counter */

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
        Crc_StartValue32 = (Crc_StartValue32 >> 1U) ^ CRC_POLYNOMIAL32;
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

  DBG_CRC_CALCULATECRC32_EXIT(Crc_StartValue32,Crc_DataPtr,Crc_Length,Crc_StartValue32,Crc_IsFirstCall);
  return Crc_StartValue32;
}

#endif

#define CRC_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
