#include "ColliderPrimitive.hpp"

namespace Engine5
{
    ColliderPrimitive::ColliderPrimitive()
        : m_mass(1.0f), m_type(eColliderType::InvalidPrimitive)
    {
    }

    ColliderPrimitive::~ColliderPrimitive()
    {
    }

    void ColliderPrimitive::CastRay(RayCastResult& result, Real max_distance)
    {
        result.hit_data.collider = this;
        Ray body_ray;
        if (m_rigid_body != nullptr)
        {
            body_ray.position  = m_rigid_body->WorldToLocalPoint(result.ray.position);
            body_ray.direction = m_rigid_body->WorldToLocalVector(result.ray.direction);
        }
        else
        {
            body_ray = result.ray;
        }
        Ray  local_ray(WorldToLocalPoint(body_ray.position), WorldToLocalVector(body_ray.direction));
        Real minimum_t = -1.0f;
        Real maximum_t = -1.0f;
        if (this->TestRayIntersection(local_ray, minimum_t, maximum_t) == true)
        {
            result.hit_data.t   = minimum_t;
            result.hit_data.hit = true;
            //ray cast done
            Vector3 local_intersection   = local_ray.position + local_ray.direction * result.hit_data.t;
            result.hit_data.intersection = result.ray.position + result.ray.direction * result.hit_data.t;
            result.hit_data.normal       = LocalToWorldVector(GetNormal(local_intersection));
            if (m_rigid_body != nullptr)
            {
                result.hit_data.normal = m_rigid_body->LocalToWorldVector(result.hit_data.normal);
            }
            if (result.hit_data.t > max_distance && max_distance >= 0.0f)
            {
                result.hit_data.hit = false;
            }
        }
    }

    void ColliderPrimitive::IntersectRay(RayIntersectionResult& result, Real max_distance)
    {
        Ray body_ray;
        if (m_rigid_body != nullptr)
        {
            body_ray.position  = m_rigid_body->WorldToLocalPoint(result.ray.position);
            body_ray.direction = m_rigid_body->WorldToLocalVector(result.ray.direction);
        }
        else
        {
            body_ray = result.ray;
        }
        Ray     local_ray(WorldToLocalPoint(body_ray.position), WorldToLocalVector(body_ray.direction));
        HitData hit_data_a;
        HitData hit_data_b;
        Real    minimum_t = -1.0f;
        Real    maximum_t = -1.0f;
        if (TestRayIntersection(local_ray, minimum_t, maximum_t) == true)
        {
            hit_data_a.t        = minimum_t;
            hit_data_a.collider = this;
            hit_data_b.t        = maximum_t;
            hit_data_b.collider = this;
            //ray cast done
            Vector3 local_intersection_a = local_ray.position + local_ray.direction * hit_data_a.t;
            hit_data_a.intersection      = result.ray.position + result.ray.direction * hit_data_a.t;
            hit_data_a.normal            = LocalToWorldVector(GetNormal(local_intersection_a));
            if (m_rigid_body != nullptr)
            {
                hit_data_a.normal = m_rigid_body->LocalToWorldVector(hit_data_a.normal);
            }
            if (Utility::IsEqual(hit_data_a.t, hit_data_b.t))
            {
                if (hit_data_a.t > max_distance || max_distance < 0.0f)
                {
                    hit_data_a.hit = true;
                    result.hit_list.push_back(hit_data_a);
                }
            }
            else
            {
                Vector3 local_intersection_b = local_ray.position + local_ray.direction * hit_data_b.t;
                hit_data_b.intersection      = result.ray.position + result.ray.direction * hit_data_b.t;
                hit_data_b.normal            = LocalToWorldVector(GetNormal(local_intersection_b));
                if (m_rigid_body != nullptr)
                {
                    hit_data_b.normal = m_rigid_body->LocalToWorldVector(hit_data_b.normal);
                }
                if (hit_data_b.t < hit_data_a.t)
                {
                    auto temp  = hit_data_a;
                    hit_data_a = hit_data_b;
                    hit_data_b = temp;
                }
                if (hit_data_a.t < max_distance || max_distance < 0.0f)
                {
                    hit_data_a.hit = true;
                    result.hit_list.push_back(hit_data_a);
                }
                if (hit_data_b.t < max_distance || max_distance < 0.0f)
                {
                    hit_data_b.hit = true;
                    result.hit_list.push_back(hit_data_b);
                }
            }
        }
    }

    Vector3 ColliderPrimitive::LocalToWorldPoint(const Vector3& local_point) const
    {
        return m_orientation.Rotate(local_point) + m_position;
    }

    Vector3 ColliderPrimitive::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_orientation.Inverse().Rotate((world_point - m_position));
    }

    Vector3 ColliderPrimitive::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_orientation.Rotate(local_vector);
    }

    Vector3 ColliderPrimitive::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_orientation.Inverse().Rotate(world_vector);
    }

    Matrix33 ColliderPrimitive::WorldInertia() const
    {
        return MassData::TranslateInertia(MassData::RotateInertia(m_local_inertia_tensor, m_orientation), m_mass, m_position);
    }

    Vector3 ColliderPrimitive::WorldCentroid() const
    {
        return m_orientation.Rotate(m_centroid) + m_position;
    }

    Vector3 ColliderPrimitive::GetBodyPosition() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_rigid_body->m_position;
        }
        return Vector3();
    }

    Quaternion ColliderPrimitive::GetBodyOrientation() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_rigid_body->m_orientation;
        }
        return Quaternion();
    }

    RigidBody* ColliderPrimitive::GetRigidBody() const
    {
        return m_rigid_body;
    }

    BoundingAABB* ColliderPrimitive::GetBoundingVolume() const
    {
        return m_bounding_volume;
    }

    ColliderSet* ColliderPrimitive::GetColliderSet() const
    {
        return m_collider_set;
    }

    Real ColliderPrimitive::GetDensity() const
    {
        return m_density;
    }

    Vector3 ColliderPrimitive::ConvertBodyWorldPoint(const Vector3& local_point) const
    {
        Vector3 result = m_orientation.Rotate(local_point);
        result += m_position;
        result = GetBodyOrientation().Rotate(result);
        result += GetBodyPosition();
        return result;
    }

    void ColliderPrimitive::SetScale(const Vector3& scale)
    {
        SetScaleData(scale);
        UpdateScaleData();
    }

    void ColliderPrimitive::SetMass(Real density)
    {
        SetMassData(density);
        UpdateMassData();
    }

    void ColliderPrimitive::SetMaterial(Physics::eMaterial material)
    {
        m_material = material;
    }

    Physics::eMaterial ColliderPrimitive::GetMaterial() const
    {
        return m_material;
    }

    eColliderType ColliderPrimitive::Type() const
    {
        return m_type;
    }

    void ColliderPrimitive::UpdatePrimitive()
    {
        if (m_collider_set != nullptr)
        {
            SetScale(m_collider_set->GetScale());
            if (m_bounding_volume != nullptr)
            {
                UpdateBoundingVolume();
            }
            SetMassData(m_density);
            m_collider_set->UpdateMassData();
            m_collider_set->UpdateColliderSetBoundingVolume();
        }
    }

    void ColliderPrimitive::UpdateMassData() const
    {
        if (m_collider_set != nullptr)
        {
            m_collider_set->UpdateMassData();
        }
    }

    void ColliderPrimitive::UpdateScaleData()
    {
        if (m_collider_set != nullptr)
        {
            if (m_bounding_volume != nullptr)
            {
                UpdateBoundingVolume();
            }
            SetMassData(m_density);
            m_collider_set->UpdateMassData();
            m_collider_set->UpdateColliderSetBoundingVolume();
        }
    }

    void ColliderPrimitive::UpdateRigidBody()
    {
        if (m_bounding_volume != nullptr)
        {
            UpdateBoundingVolume();
        }
    }
}
