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

    void ColliderSet::Initialize()
    {
        if (m_colliders == nullptr)
        {
            m_colliders = new std::vector<ColliderPrimitive*>;
        }
    }

    void ColliderSet::Update(Real dt)
    {
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


    ColliderPrimitive* ColliderSet::AddCollider(ColliderType type)
    {
        ColliderPrimitive* primitive;
        switch (type)
        {
        case Engine5::ColliderType::Box:
            primitive = new ColliderBox();
            break;
        case Engine5::ColliderType::Capsule:
            primitive = new ColliderCapsule();
            break;
        case Engine5::ColliderType::Cone:
            primitive = new ColliderCone();
            break;
        case Engine5::ColliderType::Cylinder:
            primitive = new ColliderCylinder();
            break;
        case Engine5::ColliderType::Dome:
            primitive = new ColliderDome();
            break;
        case Engine5::ColliderType::Ellipsoid:
            primitive = new ColliderEllipsoid();
            break;
        case Engine5::ColliderType::Polyhedron:
            primitive = new ColliderPolyhedron();
            break;
        case Engine5::ColliderType::Sphere:
            primitive = new ColliderSphere();
            break;
        case Engine5::ColliderType::Tetrahedron:
            primitive = new ColliderTetrahedron();
            break;
        case Engine5::ColliderType::Truncated:
            primitive = new ColliderTruncated();
            break;
        default:
            return nullptr;
        }
        primitive->m_collider_set = this;
        if (m_rigid_body != nullptr)
        {
            primitive->m_rigid_body = m_rigid_body;
        }
        if (m_world != nullptr)
        {
            m_world->AddPrimitive(primitive);
        }
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

    void ColliderSet::SetRigidBody(RigidBody* rigid_body)
    {
        m_rigid_body = rigid_body;
    }

    void ColliderSet::SetTransform(Transform* transform)
    {
        m_transform = transform;
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
        }
        if (m_transform != nullptr)
        {
            m_transform->scale = scale;
        }
    }

    MassData ColliderSet::GetMassData() const
    {
        return m_mass_data;
    }

    Vector3 ColliderSet::GetTransformScale() const
    {
        if (m_transform != nullptr)
        {
            return m_transform->scale;
        }
        return Vector3(1.0f, 1.0f, 1.0f);
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

    void ColliderSet::SyncFromTransform()
    {
        SetScale(GetTransformScale());
    }
}
