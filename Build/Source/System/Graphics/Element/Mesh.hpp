#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class BufferCommon;

    class Mesh
    {
    public:
        Mesh();
        ~Mesh();
        void Initialize();
        void Update(Real dt) const;
        void Shutdown();

    private:
        BufferCommon* m_buffer = nullptr;
    };
}
