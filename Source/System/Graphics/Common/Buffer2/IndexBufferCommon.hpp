#pragma once

#include "../../GraphicsAPI.hpp"
#include IncludeIndexBufferAPI
#include <vector>

#include "../../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class RendererCommon;

    class IndexBufferCommon : public IndexBufferAPI
    {
    public:
        IndexBufferCommon();
        ~IndexBufferCommon();

        bool Init(RendererCommon* renderer, const std::vector<U32>& indices);
        void Bind( U32 offset) const;
        void Shutdown();
    private:
    };

    
}
