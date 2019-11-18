#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    class ColliderSet
    {
    public:
        ColliderSet();
        ~ColliderSet();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:
        BoundingAABB* m_bounding_volume = nullptr;
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };

}
