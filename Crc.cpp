/**
 * @file Crc.cpp
 * @brief Enum bridge to static algorithm functions and the shared nonvirtual lifecycle.
 * @author Christof Seidel
 */
#include "Crc.h"


Crc::Crc() : CrcIf<Crc>(), mType(CRC_8) {}
Crc::~Crc() {}
Algorithm_E Crc::getType() const { return mType; }
void Crc::setType(Algorithm_E type) { reset(); mType = type; }

bool Crc::isAlgorithmEnabled() const {
    const bool enabled[] = {CRC8_ENABLED != 0U, CRC8H2F_ENABLED != 0U,
                            CRC16_ENABLED != 0U, CRC32_ENABLED != 0U, CFG_CRC8_SMBUS_ENABLE != 0U};
    const uint8_t index = static_cast<uint8_t>(mType);
    return (index < 5U) && enabled[index];
}
uint32_t Crc::calculateBlock(const uint8_t* pData, uint32_t dataLen,
                              uint32_t startValue, bool firstCall) const {
    return Crc::calculate(mType, pData, dataLen, firstCall, startValue);
}
uint32_t Crc::getDataLen() const { return CrcIf<Crc>::getDataLen(); }
uint8_t* Crc::getDataPtr() const { return CrcIf<Crc>::getDataPtr(); }
void Crc::setDataLen(uint32_t dataLen) { return CrcIf<Crc>::setDataLen(dataLen); }
void Crc::setDataPtr(uint8_t* pData) { return CrcIf<Crc>::setDataPtr(pData); }
uint32_t Crc::calculate() { return CrcIf<Crc>::calculate(); }
bool Crc::start() { return CrcIf<Crc>::start(); }
bool Crc::cancel() { return CrcIf<Crc>::cancel(); }
bool Crc::cancle() { return CrcIf<Crc>::cancel(); }
bool Crc::isFinished() { return CrcIf<Crc>::isFinished(); }
uint32_t Crc::get() { return CrcIf<Crc>::get(); }
void Crc::loop() { return CrcIf<Crc>::loop(); }
CalculationStatus_E Crc::getStatus() { return CrcIf<Crc>::getStatus(); }

uint8_t Crc::calculateCrc8(const uint8_t* pData, uint32_t dataLen,
                                      bool firstCall, uint32_t startValue) {
    uint8_t result = 0U;
#if CRC8_ENABLED == CRC_ENABLED
    result = static_cast<uint8_t>(Crc8::calculate(pData, dataLen, startValue, firstCall));
#else
    (void)pData; (void)dataLen; (void)firstCall; (void)startValue;

#endif
    return result;
}

uint8_t Crc::calculateCrc8H2F(const uint8_t* pData, uint32_t dataLen,
                                      bool firstCall, uint32_t startValue) {
    uint8_t result = 0U;
#if CRC8H2F_ENABLED == CRC_ENABLED
    result = static_cast<uint8_t>(Crc8H2F::calculate(pData, dataLen, startValue, firstCall));
#else
    (void)pData; (void)dataLen; (void)firstCall; (void)startValue;

#endif
    return result;
}

uint16_t Crc::calculateCrc16(const uint8_t* pData, uint32_t dataLen,
                                      bool firstCall, uint32_t startValue) {
    uint16_t result = 0U;
#if CRC16_ENABLED == CRC_ENABLED
    result = static_cast<uint16_t>(Crc16::calculate(pData, dataLen, startValue, firstCall));
#else
    (void)pData; (void)dataLen; (void)firstCall; (void)startValue;

#endif
    return result;
}

uint32_t Crc::calculateCrc32(const uint8_t* pData, uint32_t dataLen,
                                      bool firstCall, uint32_t startValue) {
    uint32_t result = 0U;
#if CRC32_ENABLED == CRC_ENABLED
    result = static_cast<uint32_t>(Crc32::calculate(pData, dataLen, startValue, firstCall));
#else
    (void)pData; (void)dataLen; (void)firstCall; (void)startValue;

#endif
    return result;
}

uint32_t Crc::calculate(Algorithm_E type, const uint8_t* pData, uint32_t dataLen,
                        bool firstCall, uint32_t startValue) {
    uint32_t result = 0U;
    switch (type) {
        case CRC_8: result = calculateCrc8(pData, dataLen, firstCall, startValue); break;
        case CRC_8H2F: result = calculateCrc8H2F(pData, dataLen, firstCall, startValue); break;
        case CRC_16: result = calculateCrc16(pData, dataLen, firstCall, startValue); break;
        case CRC_32: result = calculateCrc32(pData, dataLen, firstCall, startValue); break;
        case CRC_8_SMBUS: result = calculateCrc8Smbus(pData, dataLen, firstCall, startValue); break;
        default: break;
    }
    return result;
}

uint8_t Crc::calculateCrc8Smbus(const uint8_t* pData, uint32_t dataLen,
                             bool firstCall, uint32_t startValue) {
    uint8_t result = 0U;
#if CFG_CRC8_SMBUS_ENABLE == 1
    result = static_cast<uint8_t>(Crc8Smbus::calculate(pData, dataLen, startValue, firstCall));
#else
    (void)pData; (void)dataLen; (void)firstCall; (void)startValue;
#endif
    return result;
}
