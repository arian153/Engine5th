#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../BroadPhase/BoundingAABB.hpp"
#include "MassData.hpp"

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

        MassData CalculateMassData() ;
        

    private:
        friend class RigidBody;

    private:
        MassData m_mass_data;
        BoundingAABB* m_bounding_volume = nullptr;
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };

}
