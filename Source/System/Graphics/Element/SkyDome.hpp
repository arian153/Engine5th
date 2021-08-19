#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    class TextureCommon;
    class MeshData;

    class SkyDome
    {
    public:
        SkyDome();
        ~SkyDome();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:
        MeshData*      m_mesh_data = nullptr;
        TextureCommon* m_texture   = nullptr;
    };
}
