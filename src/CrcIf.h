/**
 * @file CrcIf.h
 * @note Target: 8-bit AVR and 32-bit ESP32, restricted C++11, integer arithmetic only.
 * @note Not ISR-safe: call from a superloop or task; synchronize shared instances externally.
 * @brief Nonvirtual, allocation-free abstraction for CRC configuration and progress.
 * @details Uses compile-time polymorphism (CRTP): Algorithm supplies two private
 * hooks and grants friendship to CrcIf<Algorithm>. Calls resolve to that concrete
 * type; there are no function pointers, virtual methods, tagged unions or manual
 * object construction. Each derived object owns exactly one CalculationState_T.
 */
#ifndef CRC_IF_H
#define CRC_IF_H

#include "../Crc_Types.h"

/**
 * @brief Shared configuration and cooperative lifecycle for a concrete algorithm.
 * @tparam Algorithm Derived type providing calculateBlock() and isAlgorithmEnabled().
 * @details Buffer storage is borrowed and must remain readable and unchanged while
 * processing. Copies preserve progress but share only the caller-owned buffer.
 * This class is not a runtime-polymorphic ownership interface. Its constructor
 * and destructor are protected: construct and destroy the concrete derived type.
 * @note Not ISR-safe: use from a superloop or task with external synchronization.
 * @warning Length and checksum parameters use 32-bit values on every target,
 * including 8-bit AVR. Input bytes represent serialized data.
 * @note Concurrent access to one instance requires external synchronization.
 */
template<class Algorithm>
class CrcIf {
public:
    /** @brief Read the borrowed input address. @return Configured pointer or null. */
    uint8_t* getDataPtr() const { return mState.configuration.pData; }
    /** @brief Read the total input size. @return Configured length in bytes. */
    uint32_t getDataLen() const { return mState.configuration.dataLen; }

    /**
     * @brief Change the input pointer and discard progress or an unread result.
     * @param[in] pData Borrowed readable buffer, or null. Never modified or freed.
     * @post State is CRC_NO_CALC; the configured length is preserved.
     */
    void setDataPtr(uint8_t* pData) { reset(); mState.configuration.pData = pData; }
    /**
     * @brief Change the input length and discard progress or an unread result.
     * @param[in] dataLen Number of readable bytes at the configured pointer.
     * @post State is CRC_NO_CALC; the configured pointer is preserved.
     */
    void setDataLen(uint32_t dataLen) { reset(); mState.configuration.dataLen = dataLen; }

    /**
     * @brief Calculate the configured buffer as a complete message synchronously.
     * @return Finalized CRC, or zero for invalid input or an unavailable algorithm.
     * @details Calls the concrete calculation directly and leaves cooperative
     * state unchanged. Empty blocks use the selected algorithm's empty CRC value.
     */
    uint32_t calculate() const {
        return algorithm().calculateBlock(getDataPtr(), getDataLen(), 0, true);
    }
    /**
     * @brief Start cooperative processing without consuming a byte yet.
     * @retval true Idle state, a nonempty buffer and an enabled algorithm were accepted.
     * @retval false Invalid state/configuration; existing state is unchanged.
     * @post On success, progress is cleared and status becomes CRC_CALC_ACTIVE.
     */
    bool start() {
        const bool accepted = (mState.status == CRC_NO_CALC)
            && (getDataPtr() != nullptr) && (getDataLen() != 0U)
            && algorithm().isAlgorithmEnabled();
        if (accepted) {
            reset();
            mState.status = CRC_CALC_ACTIVE;
        }
        return accepted;
    }
    /**
     * @brief Process one byte using the concrete algorithm's calculation hook.
     * @details Idle and completed instances are unchanged. The last byte immediately
     * sets CRC_CALC_FINISHED; each intermediate result is a finalized checksum.
     * @pre The input buffer remains readable and unchanged until completion/cancellation.
     */
    void loop() {
        if ((mState.status == CRC_CALC_ACTIVE) && (getDataPtr() != nullptr)
            && (mState.processedBytes < getDataLen())) {
            mState.checksum = algorithm().calculateBlock(getDataPtr() + mState.processedBytes,
                1U, mState.checksum, mState.processedBytes == 0U);
            ++mState.processedBytes;
            if (mState.processedBytes == getDataLen()) {
                mState.status = CRC_CALC_FINISHED;
            }
        }
    }
    /** @brief Inspect result readiness. @return True only for an unread completed result. */
    bool isFinished() const { return mState.status == CRC_CALC_FINISHED; }
    /** @brief Inspect the lifecycle. @return Current state without changing it. */
    CalculationStatus_E getStatus() const { return mState.status; }
    /**
     * @brief Consume the completed result and return to idle.
     * @return Finalized CRC when finished, otherwise zero with no state change.
     * @note Zero is also a valid CRC; use isFinished() to determine readiness.
     */
    uint32_t get() {
        uint32_t result = 0U;
        if (isFinished()) {
            result = mState.checksum;
            reset();
        }
        return result;
    }
    /**
     * @brief Abort active processing while retaining the buffer configuration.
     * @retval true Active progress was cleared and status is now CRC_NO_CALC.
     * @retval false No active work existed; an unread completed result is preserved.
     */
    bool cancel() {
        const bool active = (mState.status == CRC_CALC_ACTIVE);
        if (active) {
            reset();
        }
        return active;
    }
    /** @brief Legacy spelling of cancel(). @return The result of cancel(). */
    bool cancle() { return cancel(); }

    /**
     * @brief Initialize the borrowed buffer and discard previous progress.
     * @param[in] pData Readable input bytes, or nullptr for an unconfigured buffer.
     * @param[in] dataLen Available byte count; start() validates nonempty input.
     * @post The calculator is idle; the selected algorithm is unchanged.
     */
    void init(uint8_t* pData, uint32_t dataLen) {
        reset();
        mState.configuration.pData = pData;
        mState.configuration.dataLen = dataLen;
    }
    /** @brief Perform one bounded cooperative step. @see loop() */
    void process() { loop(); }

protected:
    /**
     * @brief Initialize an idle calculator without processing input.
     * @param[in] pData Borrowed input buffer, or null.
     * @param[in] dataLen Number of readable bytes; zero is permitted here.
     */
    CrcIf(uint8_t* pData = 0, uint32_t dataLen = 0)
        : mState{{pData, dataLen}, 0, 0, CRC_NO_CALC} {}
    /** @brief Allow destruction only as part of the concrete derived type. */
    ~CrcIf() = default;
    /** @brief Clear progress and become idle while retaining pointer and length. */
    void reset() {
        mState.checksum = 0;
        mState.processedBytes = 0;
        mState.status = CRC_NO_CALC;
    }

private:
    /** @brief Resolve the concrete type at compile time. @return This object's derived view. */
    const Algorithm& algorithm() const { return static_cast<const Algorithm&>(*this); }
private:
    CalculationState_T mState; ///< Sole owned state; contains no heap-managed storage.
};

#endif
