#include "ColliderSet.hpp"
#include "World.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderBox.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderCapsule.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderCone.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderCylinder.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderDome.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderEllipsoid.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderPolyhedron.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderSphere.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderTetrahedron.hpp"
#include "../ColliderPrimitive/ConvexHull3D/ColliderTruncated.hpp"

namespace Engine5
{
    ColliderSet::ColliderSet(World* world)
        : m_world(world)
    {
    }

    ColliderSet::~ColliderSet()
    {
    }

    void ColliderSet::Initialize(RigidBody* rigid_body)
    {
        if (m_colliders == nullptr)
        {
            m_colliders = new std::vector<ColliderPrimitive*>;
        }
        if (m_rigid_body == nullptr)
        {
            m_rigid_body = rigid_body;
        }
    }

    void ColliderSet::Shutdown()
    {
        if (m_colliders != nullptr)
        {
            for (auto& collider_data : *m_colliders)
            {
                collider_data->Shutdown();
                delete collider_data;
                collider_data = nullptr;
            }
            m_colliders->clear();
            delete m_colliders;
            m_colliders = nullptr;
        }
    }


    ColliderPrimitive* ColliderSet::AddCollider(eColliderType type)
    {
        ColliderPrimitive* primitive;
        switch (type)
        {
        case Engine5::eColliderType::Box:
            primitive = new ColliderBox();
            break;
        case Engine5::eColliderType::Capsule:
            primitive = new ColliderCapsule();
            break;
        case Engine5::eColliderType::Cone:
            primitive = new ColliderCone();
            break;
        case Engine5::eColliderType::Cylinder:
            primitive = new ColliderCylinder();
            break;
        case Engine5::eColliderType::Dome:
            primitive = new ColliderDome();
            break;
        case Engine5::eColliderType::Ellipsoid:
            primitive = new ColliderEllipsoid();
            break;
        case Engine5::eColliderType::Polyhedron:
            primitive = new ColliderPolyhedron();
            break;
        case Engine5::eColliderType::Sphere:
            primitive = new ColliderSphere();
            break;
        case Engine5::eColliderType::Tetrahedron:
            primitive = new ColliderTetrahedron();
            break;
        case Engine5::eColliderType::Truncated:
            primitive = new ColliderTruncated();
            break;
        default:
            return nullptr;
        }
        //each primitives must know both collider set and rigid body.
        primitive->m_collider_set = this;
        primitive->m_rigid_body   = m_rigid_body;
        m_world->AddPrimitive(primitive);
        primitive->Initialize();
        return primitive;
    }

    ColliderPrimitive* ColliderSet::GetCollider(size_t index) const
    {
        if (m_colliders != nullptr)
        {
            auto size = m_colliders->size();
            if (index < size)
            {
                return m_colliders->at(index);
            }
        }
        return nullptr;
    }

    void ColliderSet::EraseCollider(ColliderPrimitive* collider) const
    {
        auto end = m_colliders->end();
        for (auto it = m_colliders->begin(); it != end;)
        {
            if (*it == collider)
            {
                m_colliders->erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }

    RigidBody* ColliderSet::GetRigidBody() const
    {
        return m_rigid_body;
    }

    void ColliderSet::SetMass(Real density)
    {
        if (m_colliders != nullptr)
        {
            // compute local centroid & mass
            for (auto& collider_data : *m_colliders)
            {
                collider_data->SetMassData(density);
            }
            UpdateMassData();
        }
    }

    void ColliderSet::SetScale(const Vector3& scale)
    {
        if (m_colliders != nullptr)
        {
            for (auto& collider_data : *m_colliders)
            {
                collider_data->SetScaleData(scale);
                if (collider_data->m_bounding_volume != nullptr)
                {
                    collider_data->UpdateBoundingVolume();
                }
                collider_data->SetMassData(collider_data->m_density);
            }
            UpdateMassData();
            UpdateColliderSetBoundingVolume();
        }
    }

    MassData ColliderSet::GetMassData() const
    {
        return m_mass_data;
    }

    Vector3 ColliderSet::GetScale() const
    {
        return m_scale;
    }

    void ColliderSet::UpdateMassData()
    {
        m_mass_data.mass = 0.0f;
        m_mass_data.local_centroid.SetZero();
        if (m_colliders != nullptr)
        {
            // compute local centroid & mass
            for (auto& collider_data : *m_colliders)
            {
                // accumulate mass
                m_mass_data.mass += collider_data->m_mass;

                // accumulate weighted contribution
                m_mass_data.local_centroid += collider_data->m_mass * collider_data->WorldCentroid();
            }

            // compute inverse mass
            if (Utility::IsZero(m_mass_data.mass) == false)
            {
                m_mass_data.inverse_mass = 1.0f / m_mass_data.mass;
            }
            else
            {
                //infinite mass.
                m_mass_data.inverse_mass = 0.0f;
            }

            // compute final local centroid
            m_mass_data.local_centroid *= m_mass_data.inverse_mass;

            // compute local inertia tensor
            m_mass_data.local_inertia_tensor.SetZero();
            for (auto& collider_data : *m_colliders)
            {
                Vector3  r       = m_mass_data.local_centroid - collider_data->WorldCentroid();
                Real     r_dot_r = r.DotProduct(r);
                Matrix33 r_out_r = r.OuterProduct(r);

                // accumulate local inertia tensor contribution, using Parallel Axis Theorem
                m_mass_data.local_inertia_tensor += collider_data->WorldInertia() + collider_data->m_mass * (r_dot_r * Matrix33::Identity() - r_out_r);
            }

            // compute inverse inertia tensor
            m_mass_data.local_inverse_inertia_tensor = m_mass_data.local_inertia_tensor.Inverse();
        }
        if (m_rigid_body != nullptr)
        {
            m_rigid_body->SetMassData(m_mass_data);
        }
    }

    void ColliderSet::SyncToTransform(Transform* transform) const
    {
        transform->scale = m_scale;
    }

    void ColliderSet::SyncFromTransform(Transform* transform)
    {
        SetScale(transform->scale);
    }

    void ColliderSet::UpdateColliderSetBoundingVolume()
    {
        if (m_colliders != nullptr)
        {
            m_bounding_volume.Set(Math::Vector3::ORIGIN, Math::Vector3::ORIGIN);

            for (auto& collider_data : *m_colliders)
            {
                m_bounding_volume = collider_data->m_bounding_volume->Union(m_bounding_volume);
            }
        }
    }

}
