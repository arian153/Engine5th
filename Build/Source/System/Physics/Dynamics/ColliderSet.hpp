#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../BroadPhase/BoundingAABB.hpp"
#include "MassData.hpp"

namespace Engine5
{
    class RigidBody;
    enum class ColliderType;
    class ColliderPrimitive;

    class ColliderSet
    {
    public:
        ColliderSet();
        ~ColliderSet();

        void Initialize();
        void Update(Real dt);
        void Shutdown();


        void               SetRigidBody(RigidBody* rigid_body);
        ColliderPrimitive* AddCollider(ColliderType type);

        void     CalculateMassData();
        MassData GetMassData() const;

        void UpdateScale();
        Vector3 GetTransformScale() const;

    private:
        friend class RigidBody;

    private:
        MassData      m_mass_data;
        BoundingAABB* m_bounding_volume = nullptr;
        RigidBody*    m_rigid_body      = nullptr;
        Transform*    m_transform       = nullptr;

        //primitives
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };
}
