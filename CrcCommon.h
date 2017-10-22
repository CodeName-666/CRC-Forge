/*
 * CrcCommon.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC_CRCCOMMON_H_
#define _CRC_CRCCOMMON_H_


/**
 *  \brief Number of elements in CRC8 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC_8_TABLE_SIZE      256U

/**
 * \brief Number of elements in CRC8H2F lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC_8H2F_TABLE_SIZE   0U

/**
 * \brief Number of elements in CRC16 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC_16_TABLE_SIZE     256U

/**
 * \brief Number of elements in CRC32 lookup table
 *
 * If size is 0 table based calculation is deactivated. */
#define CRC_32_TABLE_SIZE     256U


class CrcCommon
{
   public:
      CrcCommon();
      virtual ~CrcCommon();
};

#endif /* _CRC_CRCCOMMON_H_ */
