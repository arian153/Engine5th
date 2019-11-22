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
        void SetMass(Real density);
        void SetScale(const Vector3& scale);

        //getters
        MassData GetMassData() const;
        Vector3  GetTransformScale() const;

        //modify data
        void UpdateMassData();
        void SyncFromTransform();

    private:


    private:
        friend class RigidBody;
        friend class ColliderComponent;

    private:
        RigidBody*    m_rigid_body      = nullptr;
        Transform*    m_transform       = nullptr;
        MassData      m_mass_data;
        Vector3       m_scale;

        //primitives
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };
}
