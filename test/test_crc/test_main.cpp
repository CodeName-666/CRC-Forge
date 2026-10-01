#include <Crc.h>
#include <Crc_Types.h>
#include <src/CrcIf.h>
#include <type_traits>
#include <unity.h>

static uint8_t message[] = "123456789";
static const uint32_t expected[] = {0x4BU, 0xDFU, 0x29B1U, 0xCBF43926UL, 0xF4U};
static const bool enabled[] = {CRC8_ENABLED, CRC8H2F_ENABLED, CRC16_ENABLED, CRC32_ENABLED, CFG_CRC8_SMBUS_ENABLE};
void setUp() {}
void tearDown() {}

void reference_vectors() {
    for (unsigned i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_HEX32(enabled[i] ? expected[i] : 0,
            Crc::calculate(static_cast<Algorithm_E>(i), message, 9));
    }
}

void every_split_preserves_crc() {
    for (unsigned i = 0; i < 5; ++i) {
        if (!enabled[i]) continue;
        for (unsigned split = 0; split <= 9; ++split) {
            const Algorithm_E type = static_cast<Algorithm_E>(i);
            const uint32_t prefix = Crc::calculate(type, message, split);
            TEST_ASSERT_EQUAL_HEX32(expected[i],
                Crc::calculate(type, message + split, 9 - split, false, prefix));
        }
    }
}

void cooperative_calculation() {
    for (unsigned i = 0; i < 5; ++i) {
        Crc crc;
        crc.setType(static_cast<Algorithm_E>(i));
        crc.setDataPtr(message);
        crc.setDataLen(9);
        TEST_ASSERT_EQUAL(enabled[i], crc.start());
        if (!enabled[i]) continue;
        TEST_ASSERT_FALSE(crc.start());
        for (unsigned byte = 0; byte < 9; ++byte) crc.loop();
        TEST_ASSERT_TRUE(crc.isFinished());
        TEST_ASSERT_EQUAL_HEX32(expected[i], crc.get());
        TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
        TEST_ASSERT_EQUAL_UINT32(0, crc.get());
    }
}

void invalid_inputs_and_cancellation() {
    Crc crc;
    TEST_ASSERT_FALSE(crc.start());
    TEST_ASSERT_FALSE(crc.cancle());
    TEST_ASSERT_EQUAL_UINT32(0, Crc::calculate(CRC_32, 0, 9));
    TEST_ASSERT_EQUAL_UINT32(0, Crc::calculate(static_cast<Algorithm_E>(99), message, 9));
    for (unsigned i = 0; i < 5; ++i) {
        if (!enabled[i]) continue;
        crc.setType(static_cast<Algorithm_E>(i));
        crc.setDataPtr(message);
        crc.setDataLen(9);
        TEST_ASSERT_TRUE(crc.start());
        crc.loop();
        TEST_ASSERT_TRUE(crc.cancle());
        TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
        TEST_ASSERT_TRUE(crc.start());
        // Reconfiguration must cancel the active calculation safely.
        crc.setDataPtr(0);
        crc.loop();
        TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
    }
}

void binary_data_and_bytewise_continuation() {
    uint8_t bytes[256];
    for (unsigned i = 0; i < 256; ++i) bytes[i] = static_cast<uint8_t>(i);
    // Independently calculated fixtures; CRC16/32 cross-checked with Python
    // binascii.crc_hqx and zlib.crc32 respectively.
    const uint32_t binaryExpected[] = {0x05, 0x06, 0x3FBD, 0x29058C73UL, 0x14U};
    for (unsigned i = 0; i < 5; ++i) {
        if (!enabled[i]) continue;
        const Algorithm_E type = static_cast<Algorithm_E>(i);
        TEST_ASSERT_EQUAL_HEX32(binaryExpected[i], Crc::calculate(type, bytes, 256));
        uint32_t crc = 0;
        for (unsigned byte = 0; byte < 256; ++byte)
            crc = Crc::calculate(type, bytes + byte, 1, byte == 0, crc);
        TEST_ASSERT_EQUAL_HEX32(binaryExpected[i], crc);
    }
}

void const_inputs_empty_blocks_and_direct_api() {
    const uint8_t input[] = "123456789";
    TEST_ASSERT_EQUAL_HEX32(enabled[0] ? expected[0] : 0, Crc::calculateCrc8(input, 9));
    TEST_ASSERT_EQUAL_HEX32(enabled[1] ? expected[1] : 0, Crc::calculateCrc8H2F(input, 9));
    TEST_ASSERT_EQUAL_HEX32(enabled[2] ? expected[2] : 0, Crc::calculateCrc16(input, 9));
    TEST_ASSERT_EQUAL_HEX32(enabled[3] ? expected[3] : 0, Crc::calculateCrc32(input, 9));
    const uint32_t empty[] = {0, 0, 0xFFFF, 0, 0};
    for (unsigned i = 0; i < 5; ++i) {
        const Algorithm_E type = static_cast<Algorithm_E>(i);
        TEST_ASSERT_EQUAL_HEX32(enabled[i] ? empty[i] : 0, Crc::calculate(type, 0, 0));
        TEST_ASSERT_EQUAL_HEX32(enabled[i] ? expected[i] : 0,
            Crc::calculate(type, 0, 0, false, expected[i]));
        TEST_ASSERT_EQUAL_HEX32(enabled[i] ? expected[i] : 0,
            Crc::calculate(type, input, 9, true, 0x12345678UL));
        TEST_ASSERT_EQUAL_HEX32(0, Crc::calculate(type, 0, 1));
    }
}

void configured_api_and_cancel_alias() {
    Crc crc;
    crc.setDataPtr(message);
    crc.setDataLen(9);
    crc.setType(static_cast<Algorithm_E>(99));
    TEST_ASSERT_FALSE(crc.start());
    for (unsigned i = 0; i < 5; ++i) {
        if (!enabled[i]) continue;
        const Algorithm_E type = static_cast<Algorithm_E>(i);
        crc.setType(type);
        TEST_ASSERT_EQUAL(type, crc.getType());
        TEST_ASSERT_EQUAL_PTR(message, crc.getDataPtr());
        TEST_ASSERT_EQUAL_UINT32(9, crc.getDataLen());
        TEST_ASSERT_EQUAL_HEX32(expected[i], crc.calculate());
        TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
        TEST_ASSERT_TRUE(crc.start());
        TEST_ASSERT_EQUAL_HEX32(0, crc.get());
        TEST_ASSERT_TRUE(crc.cancel());
        TEST_ASSERT_FALSE(crc.cancel());
        TEST_ASSERT_TRUE(crc.start());
        crc.setDataLen(0);
        TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
        TEST_ASSERT_FALSE(crc.start());
        crc.setDataLen(9);
        TEST_ASSERT_TRUE(crc.start());
        crc.setType(type);
        TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
    }
}

void subclass_static_calculations() {
#if CRC8_ENABLED == CRC_ENABLED
    TEST_ASSERT_EQUAL_HEX32(0x4B, Crc8::calculate(message, 9));
#endif
#if CRC8H2F_ENABLED == CRC_ENABLED
    TEST_ASSERT_EQUAL_HEX32(0xDF, Crc8H2F::calculate(message, 9));
#endif
#if CRC16_ENABLED == CRC_ENABLED
    TEST_ASSERT_EQUAL_HEX32(0x29B1, Crc16::calculate(message, 9));
#endif
#if CRC32_ENABLED == CRC_ENABLED
    TEST_ASSERT_EQUAL_HEX32(0xCBF43926UL, Crc32::calculate(message, 9));
#endif
}

template<class Algorithm>
void check_algorithm_instance(uint32_t expectedValue, uint32_t emptyValue) {
    Algorithm crc;
    const CalculationStatus_E initialStatus = crc.getStatus();
    TEST_ASSERT_EQUAL(CRC_NO_CALC, initialStatus);
    TEST_ASSERT_FALSE(crc.start());
    TEST_ASSERT_EQUAL_HEX32(emptyValue, crc.calculate());
    crc.setDataPtr(message);
    crc.setDataLen(9);
    TEST_ASSERT_EQUAL_PTR(message, crc.getDataPtr());
    TEST_ASSERT_EQUAL_UINT32(9, crc.getDataLen());
    TEST_ASSERT_EQUAL_HEX32(expectedValue, crc.calculate());
    TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
    TEST_ASSERT_TRUE(crc.start());
    TEST_ASSERT_FALSE(crc.start());
    crc.loop();
    TEST_ASSERT_EQUAL_HEX32(0, crc.get());
    // A synchronous static call must not disturb cooperative progress.
    TEST_ASSERT_EQUAL_HEX32(expectedValue, Algorithm::calculate(message, 9));
    Algorithm copied = crc;
    for (unsigned i = 1; i < 9; ++i) { crc.loop(); copied.loop(); }
    TEST_ASSERT_TRUE(crc.isFinished());
    TEST_ASSERT_FALSE(crc.start());
    TEST_ASSERT_FALSE(crc.cancel());
    TEST_ASSERT_EQUAL_HEX32(expectedValue, crc.get());
    TEST_ASSERT_EQUAL_HEX32(expectedValue, copied.get());
    TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
    TEST_ASSERT_TRUE(crc.start());
    crc.loop();
    TEST_ASSERT_TRUE(crc.cancle());
    TEST_ASSERT_FALSE(crc.cancel());
    TEST_ASSERT_TRUE(crc.start());
    crc.setDataLen(0);
    TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
    TEST_ASSERT_FALSE(crc.start());
    crc.setDataLen(9);
    TEST_ASSERT_TRUE(crc.start());
    crc.setDataPtr(0);
    crc.loop();
    TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
    TEST_ASSERT_EQUAL_HEX32(0, crc.calculate());
    for (unsigned split = 0; split <= 9; ++split) {
        uint32_t partial = Algorithm::calculate(message, split);
        TEST_ASSERT_EQUAL_HEX32(expectedValue,
            Algorithm::calculate(message + split, 9 - split, partial, false));
    }
}

void subclass_instances() {
#if CFG_CRC8_SMBUS_ENABLE == 1
    check_algorithm_instance<Crc8Smbus>(0xF4U, 0U);
#endif
#if CRC8_ENABLED == CRC_ENABLED
    check_algorithm_instance<Crc8>(0x4B, 0);
#endif
#if CRC8H2F_ENABLED == CRC_ENABLED
    check_algorithm_instance<Crc8H2F>(0xDF, 0);
#endif
#if CRC16_ENABLED == CRC_ENABLED
    check_algorithm_instance<Crc16>(0x29B1, 0xFFFF);
#endif
#if CRC32_ENABLED == CRC_ENABLED
    check_algorithm_instance<Crc32>(0xCBF43926UL, 0);
#endif
}

void bridge_switching_and_copying() {
    Crc crc;
    const CalculationStatus_E initialStatus = crc.getStatus();
    TEST_ASSERT_EQUAL(CRC_NO_CALC, initialStatus);
    crc.setDataPtr(message);
    crc.setDataLen(9);
    for (unsigned from = 0; from < 6; ++from) {
        for (unsigned to = 0; to < 6; ++to) {
            crc.setType(static_cast<Algorithm_E>(from));
            crc.start();
            crc.loop();
            crc.setType(static_cast<Algorithm_E>(to));
            TEST_ASSERT_EQUAL_PTR(message, crc.getDataPtr());
            TEST_ASSERT_EQUAL_UINT32(9, crc.getDataLen());
            TEST_ASSERT_EQUAL(CRC_NO_CALC, crc.getStatus());
            const bool available = to < 5 && enabled[to];
            TEST_ASSERT_EQUAL(available, crc.start());
            crc.loop();
            Crc copied(crc);
            Crc assigned;
            assigned = crc;
            for (unsigned i = 1; i < 9; ++i) {
                crc.loop(); copied.loop(); assigned.loop();
            }
            const uint32_t result = available ? expected[to] : 0;
            TEST_ASSERT_EQUAL_HEX32(result, crc.get());
            TEST_ASSERT_EQUAL_HEX32(result, copied.get());
            TEST_ASSERT_EQUAL_HEX32(result, assigned.get());
        }
    }
}

void shared_nonvirtual_interface() {
    static_assert(!std::is_polymorphic<Crc>::value,
                  "The CRC bridge must not require virtual dispatch");
#if CRC16_ENABLED == CRC_ENABLED
    Crc16 algorithm(message, 9);
    CrcIf<Crc16>& algorithmInterface = algorithm;
    TEST_ASSERT_EQUAL_HEX32(0x29B1, algorithmInterface.calculate());
    TEST_ASSERT_TRUE(algorithmInterface.start());
    for (unsigned i = 0; i < 9; ++i) algorithmInterface.loop();
    TEST_ASSERT_EQUAL_HEX32(0x29B1, algorithmInterface.get());
#endif
    Crc bridge;
    CrcIf<Crc>& interface = bridge;
    interface.setDataPtr(message);
    interface.setDataLen(9);
    for (unsigned i = 0; i < 5; ++i) {
        bridge.setType(static_cast<Algorithm_E>(i));
        TEST_ASSERT_EQUAL(enabled[i], interface.start());
        for (unsigned byte = 0; byte < 9; ++byte) interface.loop();
        TEST_ASSERT_EQUAL_HEX32(enabled[i] ? expected[i] : 0, interface.get());
    }
}

void bounded_process_and_reinitialization() {
    static_assert(sizeof(Algorithm_E) == 1U, "Algorithm enum must use one byte");
    static_assert(sizeof(CalculationStatus_E) == 1U, "Status must use one byte");
    Crc calculator;
    for (uint8_t type = 0U; type < 5U; ++type) {
        calculator.setType(static_cast<Algorithm_E>(type));
        calculator.init(message, 9U);
        calculator.process();
        TEST_ASSERT_EQUAL(CRC_NO_CALC, calculator.getStatus());
        TEST_ASSERT_EQUAL(enabled[type], calculator.start());
        for (uint8_t byte = 0U; byte < 8U; ++byte) {
            calculator.process();
            TEST_ASSERT_FALSE(calculator.isFinished());
        }
        calculator.process();
        calculator.process();
        TEST_ASSERT_EQUAL(enabled[type], calculator.isFinished());
        TEST_ASSERT_EQUAL_HEX32(enabled[type] ? expected[type] : 0U, calculator.get());
        (void)calculator.start();
        calculator.process();
        calculator.init(nullptr, 9U);
        calculator.process();
        TEST_ASSERT_FALSE(calculator.start());
        TEST_ASSERT_EQUAL(CRC_NO_CALC, calculator.getStatus());
    }
}

/** @brief Independently divide a byte polynomial by x^8+x^2+x+1. */
void smbus_exhaustive_update() {
    TEST_ASSERT_EQUAL_HEX32(CFG_CRC8_SMBUS_ENABLE ? 0xF4U : 0U,
        Crc::calculateCrc8Smbus(message, 9U));
#if CFG_CRC8_SMBUS_ENABLE == 1
    for (uint16_t seed = 0U; seed < 256U; ++seed) {
        for (uint16_t value = 0U; value < 256U; ++value) {
            const uint8_t input = static_cast<uint8_t>(value);
            uint16_t dividend = static_cast<uint16_t>((seed ^ value) << 8U);
            for (uint8_t bit = 0U; bit < 8U; ++bit) {
                const uint16_t leading = static_cast<uint16_t>(0x8000U >> bit);
                if ((dividend & leading) != 0U) {
                    dividend = static_cast<uint16_t>(dividend ^ (0x107U << (7U - bit)));
                }
            }
            TEST_ASSERT_EQUAL_HEX32(dividend, Crc8Smbus::calculate(&input, 1U, seed, false));
        }
    }
    TEST_ASSERT_EQUAL_HEX32(0xABU, Crc8Smbus::calculate(nullptr, 0U, 0x12ABU, false));
    TEST_ASSERT_EQUAL_HEX32(0U, Crc8Smbus::calculate(nullptr, 1U));
    TEST_ASSERT_EQUAL_HEX32(0U, Crc8Smbus::calculate(nullptr, 0U));
#endif
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(bounded_process_and_reinitialization);
    RUN_TEST(smbus_exhaustive_update);
    RUN_TEST(reference_vectors);
    RUN_TEST(every_split_preserves_crc);
    RUN_TEST(cooperative_calculation);
    RUN_TEST(invalid_inputs_and_cancellation);
    RUN_TEST(binary_data_and_bytewise_continuation);
    RUN_TEST(const_inputs_empty_blocks_and_direct_api);
    RUN_TEST(configured_api_and_cancel_alias);
    RUN_TEST(subclass_static_calculations);
    RUN_TEST(subclass_instances);
    RUN_TEST(bridge_switching_and_copying);
    RUN_TEST(shared_nonvirtual_interface);
    return UNITY_END();
}
