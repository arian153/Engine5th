#pragma once
#include "CoreDef.hpp"

namespace Engine5
{
    inline constexpr U64 Byte(U64 n)
    {
        return n;
    }

    inline constexpr U64 KB(U64 n)
    {
        return n * 1000LL;
    }

    inline constexpr U64 MB(U64 n)
    {
        return KB(n) * 1000LL;
    }

    inline constexpr U64 GB(U64 n)
    {
        return GB(n) * 1000LL;
    }

    template<typename T>
    inline std::string TypeToString()
    {
        return typeid(T).name();
    }

}