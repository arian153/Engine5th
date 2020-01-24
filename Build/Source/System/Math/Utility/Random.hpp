#pragma once
#include <random>
#include "../../Core/Utility/CoreDef.hpp"
#include "MathDef.hpp"

namespace Engine5
{
    class Random
    {
    public:
        Random();
        Random(U32 seed);
        ~Random();

        void SetSeed(U32 seed);
        U32 GetSeed() const;
        U32 GenerateSeed();

        U32 GetRandomU32();
        U64 GetRandomU64();
        I32 GetRandomI32();
        I64 GetRandomI64();
        R64 GetRandomR64();

        Real GetRandomReal();

        U32 GetRangedRandomU32(U32 min, U32 max);
        U64 GetRangedRandomU64(U64 min, U64 max);
        I32 GetRangedRandomI32(I32 min, I32 max);
        I64 GetRangedRandomI64(I64 min, I64 max);
        R64 GetRangedRandomR64(R64 min, R64 max);
        Real GetRangedRandomReal(Real min, Real max);

        bool IsNPercentSuccessReal(Real percent);
        bool IsNPercentSuccessR64(R64 percent);

        I32 NPercentSuccessMTimesResult(I32 m, Real percent);
        U32 GetNormalDistributionU32(U32 average, U32 deviation);
        U64 GetNormalDistributionU64(U64 average, U64 deviation);
        I32 GetNormalDistributionI32(I32 average, I32 deviation);
        I64 GetNormalDistributionI64(I64 average, I64 deviation);
        R64 GetNormalDistributionR64(R64 average, R64 deviation);
        Real GetNormalDistributionReal(Real average, Real deviation);

    private:
        U32 m_seed;
        std::mt19937 m_random32;
        std::mt19937_64 m_random64;
        std::random_device m_rng;
    };

}