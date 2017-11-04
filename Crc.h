/*
 * Crc.h
 *
 *  Created on: 22.10.2017
 *      Author: AP02
 */

#ifndef _CRC_H_
#define _CRC_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include "arduino.h"
#else
   #include "WProgram.h"
#endif

#include "Crc_Cfg.h"

#if (CRC8_ENABLED == CRC_ENABLED)
#include "src/Crc8.h"
#endif

#if(CRC8H2F_ENABLED == CRC_ENABLED)
#include "src/Crc8H2F.h"
#endif

#if(CRC16_ENABLED == CRC_ENABLED)
#include "src/Crc16.h"
#endif

#if(CRC32_ENABLED == CRC_ENABLED)
#include "src/Crc32.h"
#endif


class Crc
{
   public:
   /**
    * @brief CRC Type
    *
    * This enumerate is used as type, which crc checksum
    * will be calculated.
    */
   typedef enum
   {
      CRC_8          = 0x00,//!< CRC_8 calculation will be used.
      CRC_8H2F             ,//!< CRC_8H2F calculation will be used.
      CRC16                ,//!< CRC16 calculation will be used.
      CRC32                 //!< CRC32 calculation will be used.
   }Crc_t;

   /**
    * @brief Calculation Status
    *
    * This enumerate defines all status values,
    * which will be returned during crc calculation.
    */
   typedef enum
   {
      CRC_NO_CALC         = 0x00,//!< CRC_NO_CALC - No calculation is ongoing.
      CRC_CALC_ACTIVE           ,//!< CRC_CALC_ACTIVE - Crc calculation is ongoing.
      CRC_CALC_FINISHED         ,//!< CRC_CALC_FINISHED - Crc calculation is finished.

   }Crc_CalcStatus_t;


   public:  /*Methods*/
      /**
       * @brief Default Constructor
       *
       */
      Crc();

      /**
       * @brief Destructor
       *
       */
      virtual ~Crc();

      /**
       * @brief Get Data Length
       *
       * This getter method returns the configured data length.
       *
       * @return Length of data.
       */
      uint32_t getDataLen() const;

      /**
       * @brief Get Data Pointer
       * This getter method returns a pointer to the data, of which the crc will be
       * calculated
       *
       * @return Pointer to data.
       */
      uint8_t* getDataPtr() const;

      /**
       * @brief Get Type
       * Returns the configured CRC Type. @see Crc_t.
       *
       * @return Type which will be used.
       */
      Crc_t getType() const;

      /**
       * @brief Set Data Length
       * Setter method to set the data length of the data pointer.
       * @param dataLen Length of data pointer.
       */
      void setDataLen(uint32 dataLen);

      /**
       * @brief Set Data Pointer
       * Setter method to set a pointer to the data, to calculate the crc.
       * @param dataPtr Pointer to data.
       */
      void setDataPtr(uint8_t* dataPtr);

      /**
       * @Set Type
       *
       * Setter to configure which CRC type should be used. @see Crc_t
       * @param type Type which should be used.
       */
      void setType(Crc_t type);

      /**
       * @brief Calculate
       * Calculates immediately the CRC checksum. Used will be the current
       * configuration.
       * @return Crc checksum
       */
      uint32_t calculate(void);

      /**
       * @brief Start
       *
       * Indication to calculate the crc checksum in a loop.
       */
      boolean start(void);

      /**
       *
       * @return
       */
      boolean cancle(void);

      /**
       * @brief Is Finished
       * @return true Crc calculation finished
       * @return false Crc calculation not finished.
       */
      boolean isFinished(void);

      /**
       * @brief Get
       * Getter method which returns the calculated crc.
       * @return Crc Checksum
       */
      uint32_t get(void);

      /**
       * @brief Loop
       */
      void loop(void);

      /**
       * @brief Get Status
       * @return
       */
      Crc_CalcStatus_t getStatus(void);

   public: /*Static Methods*/
      /**
       * @brief Calculate CRC 82HF
       * @param dataPtr
       * @param dataLen
       * @param firstCall
       * @param startValue
       * @return
       */
      static uint8_t  calculateCrc82HF(uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE);
      /**
       * @brief Calculate CRC 8
       * @param dataPtr
       * @param dataLen
       * @param firstCall
       * @param startValue
       * @return
       */
      static uint8_t  calculateCrc8   (uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE);
      /**
       * @brief Calculate CRC 16
       *
       * @param dataPtr
       * @param dataLen
       * @param firstCall
       * @param startValue
       * @return
       */
      static uint16_t calculateCrc16  (uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE);
      /**
       * @brief Calculate CRC 32
       * @param dataPtr
       * @param dataLen
       * @param firstCall
       * @param startValue
       * @return
       */
      static uint32_t calculateCrc32  (uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE);
      /**
       * @brief Calculate
       * @param type
       * @param dataPtr
       * @param dataLen
       * @param firstCall
       * @param startValue
       * @return
       */
      static uint32_t calculate       (Crc_t type, uint8_t* dataPtr, uint32_t dataLen, boolean firstCall = true, uint32 startValue = CRC_START_VALUE);

   private: /*Parameter*/
      uint8_t*         _dataPtr;
      uint32_t         _dataLen;
      uint32_t         _crc;
      Crc_t            _type;
      uint32_t         _dataCount;
      Crc_CalcStatus_t _status;

};

#endif /* SOUCRE_CRC_CRC_H_ */
