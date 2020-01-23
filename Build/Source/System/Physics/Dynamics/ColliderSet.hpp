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

    private:


    private:
        friend class World;
        friend class RigidBody;
        friend class ColliderComponent;
        friend class CollisionDataTable;

    private:
        RigidBody* m_rigid_body = nullptr;
        World*     m_world      = nullptr;
        MassData   m_mass_data;
        Vector3    m_scale;

        //primitives
        std::vector<ColliderPrimitive*>* m_colliders = nullptr;
    };
}
