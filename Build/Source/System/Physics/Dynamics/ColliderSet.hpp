#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "../BroadPhase/BoundingAABB.hpp"
#include "MassData.hpp"

namespace Engine5
{
    class RigidBody;
    enum class ColliderType;
    class ColliderPrimitive;
    class World;

    class ColliderSet
    {
    public:
        explicit ColliderSet(World* world);
        ~ColliderSet();

        void Initialize(RigidBody* rigid_body);
        void Shutdown();

        //primitives
        ColliderPrimitive* AddCollider(ColliderType type);
        ColliderPrimitive* GetCollider(size_t index) const;
        void EraseCollider(ColliderPrimitive* collider) const;

        RigidBody* GetRigidBody();

        //setters
        void SetMass(Real density);
        void SetScale(const Vector3& scale);

        //getters
        MassData GetMassData() const;
        Vector3 GetScale() const;

        //modify data
        void UpdateMassData();
        void SyncToTransform(Transform* transform) const;
        void SyncFromTransform(Transform* transform);
        void UpdateColliderSetBoundingVolume();

    private:


    private:
        friend class World;
        friend class RigidBody;
        friend class ColliderComponent;
        friend class FillteringPhase;
        friend class ContactManifold;

    private:
        RigidBody* m_rigid_body = nullptr;
        World*     m_world      = nullptr;
        MassData   m_mass_data;
        Vector3    m_scale;
        BoundingAABB m_bounding_volume;

        //primitives
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };
}
