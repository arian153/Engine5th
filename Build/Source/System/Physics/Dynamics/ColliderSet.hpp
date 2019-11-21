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

        //primitives
        ColliderPrimitive* AddCollider(ColliderType type);
        ColliderPrimitive* GetCollider(size_t index) const;

        //Set pointer to body, transform
        void SetRigidBody(RigidBody* rigid_body);
        void SetTransform(Transform* transform);

        //setters
        void SetMassData(Real density);
        void SetScale(const Vector3& scale);
        void UpdateMassData();
        void UpdateScale();

        //getters
        MassData GetMassData() const;
        Vector3  GetTransformScale() const;


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
