/**
 * @file main.cpp
 * @brief Select CRC32 through the runtime enum bridge.
 * @details Demonstrates synchronous, blockwise and cooperative calculation.
 * Arduino performs one byte per loop() call; the host checks the same lifecycle.
 * @note Run from a superloop or task, not from an ISR. The static buffer remains
 * valid and unchanged until the calculation finishes.
 */
#include <Crc.h>

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdio.h>
#endif

/** @brief Serialized reference message; the terminator is not processed. */
static uint8_t sData[] = "123456789";
/** @brief Message length in bytes. */
static constexpr uint32_t DATA_LENGTH = static_cast<uint32_t>(sizeof(sData) - 1U);
/** @brief Published check value for the reference message. */
static constexpr uint32_t EXPECTED_CRC = 0xCBF43926UL;
/** @brief Calculator with static storage and no dynamic allocation. */
static Crc sChecksum;

/**
 * @brief Calculate two consecutive blocks using static algorithm calls.
 * @return Finalized CRC of the complete reference message.
 * @details The bridge takes the first-call flag before the previous checksum.
 */
static uint32_t calculateBlocks() {
    const uint32_t first = Crc::calculate(CRC_32, sData, 4U);
    return Crc::calculate(CRC_32, sData + 4U, DATA_LENGTH - 4U, false, first);
}

/**
 * @brief Configure the message, verify synchronous modes and start processing.
 * @return True if both reference checks pass and cooperative processing starts.
 * @post On success, no input byte has been processed cooperatively yet.
 */
static bool initializeExample() {
    sChecksum.setType(CRC_32);
    sChecksum.init(sData, DATA_LENGTH);
    const bool valid = (calculateBlocks() == EXPECTED_CRC)
        && (sChecksum.calculate() == EXPECTED_CRC);
    return valid && sChecksum.start();
}

#ifdef ARDUINO
/** @brief Initialize serial output and start the reference calculation once. */
void setup() {
    Serial.begin(115200UL);
    if (!initializeExample()) {
        Serial.println(F("CRC initialization or reference check failed"));
    }
}

/**
 * @brief Process at most one byte and print the result once when ready.
 * @details Other application work can run after process(); no waiting is needed.
 * get() consumes the completed result and returns the calculator to idle.
 */
void loop() {
    sChecksum.process();
    const CalculationStatus_E status = sChecksum.getStatus();
    if (status == CRC_CALC_FINISHED) {
        const uint32_t result = sChecksum.get();
        Serial.print(F("CRC32: "));
        Serial.println(result, HEX);
        if (result != EXPECTED_CRC) {
            Serial.println(F("CRC reference check failed"));
        }
    }
}
#else
/**
 * @brief Execute a bounded host check of the example without hardware.
 * @return Zero on success; one if initialization, completion or checksum fails.
 * @note The standard C++ host entry point requires an int return type.
 */
int main() {
    bool valid = initializeExample();
    if (valid) {
        for (uint32_t byte = 0U; byte < DATA_LENGTH; ++byte) {
            sChecksum.process();
        }
        valid = (sChecksum.getStatus() == CRC_CALC_FINISHED);
        const uint32_t result = sChecksum.get();
        valid = valid && (result == EXPECTED_CRC)
            && (sChecksum.getStatus() == CRC_NO_CALC);
    }
    (void)puts(valid ? "CRC32: CBF43926 (all modes passed)" : "CRC example failed");
    return valid ? 0 : 1;
}
#endif
