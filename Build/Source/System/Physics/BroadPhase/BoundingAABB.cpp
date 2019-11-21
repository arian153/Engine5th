#include "BoundingAABB.hpp"
#include "..//ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    BoundingAABB::BoundingAABB(ColliderPrimitive* collider)
        : m_collider(collider)
    {
        if (m_collider != nullptr)
        {
            m_collider->m_bounding_volume = this;
            m_collider_set                = m_collider->m_collider_set;
        }
    }

    BoundingAABB::BoundingAABB(const BoundingAABB& a, const BoundingAABB& b)
    {
        Vector3 offset      = b.Center() - a.Center();
        Real    a_half_size = 0.5f * (a.Size().Length());
        Real    b_half_size = 0.5f * (b.Size().Length());
        Real    size_diff   = b_half_size - a_half_size;
        if (size_diff * size_diff >= offset.LengthSquared())
        {
            if (a_half_size > b_half_size)
            {
                this->m_min = a.m_min; //a.Center() - a.HalfSize();
                this->m_max = a.m_max; //a.Center() + a.HalfSize();
            }
            else
            {
                this->m_min = b.m_min; //b.Center() - b.HalfSize();
                this->m_max = b.m_max; //b.Center() + b.HalfSize();
            }
        }
        else
        {
            Real    distance    = offset.Length();
            Vector3 offset_size = offset;
            if (offset.x < 0.0f)
            {
                offset_size.x = -offset.x;
            }
            if (offset.y < 0.0f)
            {
                offset_size.y = -offset.y;
            }
            if (offset.z < 0.0f)
            {
                offset_size.z = -offset.z;
            }
            Vector3 half_size = (a.HalfSize() + b.HalfSize() + offset_size) * 0.5f;
            Vector3 center    = a.Center();
            if (distance > 0.0f)
            {
                center += offset * ((half_size.Length() - a_half_size) / distance);
            }
            this->m_min = center - half_size;
            this->m_max = center + half_size;
        }
    }

    BoundingAABB::~BoundingAABB()
    {
    }

    void BoundingAABB::Set(const Vector3& min, const Vector3& max)
    {
        m_min = min;
        m_max = max;
    }

    bool BoundingAABB::Intersect(BoundingAABB* aabb) const
    {
        // if separated in x direction
        if (m_min.x > aabb->m_max.x || aabb->m_min.x > m_max.x)
            return false;

        // if separated in y direction
        if (m_min.y > aabb->m_max.y || aabb->m_min.y > m_max.y)
            return false;

        // if separated in z direction
        if (m_min.z > aabb->m_max.z || aabb->m_min.z > m_max.z)
            return false;

        // if same collider set, skip intersection
        if (m_collider_set != nullptr && m_collider_set == aabb->m_collider_set)
        {
            return false;
        }

        // no separation, must be intersecting
        return true;
    }

    bool BoundingAABB::Contains(const Vector3& point) const
    {
        if (m_min.x > point.x || point.x > m_max.x)
        {
            return false;
        }
        if (m_min.y > point.y || point.y > m_max.y)
        {
            return false;
        }
        if (m_min.z > point.z || point.z > m_max.z)
        {
            return false;
        }
        return true;
    }

    bool BoundingAABB::TestRayIntersection(const Ray& ray, Real& t, Real max_distance) const
    {
        // do tests against three sets of planes
        Real    t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;
        Vector3 box_min = m_min;
        Vector3 box_max = m_max;
        Vector3 inv_dir = ray.direction;
        inv_dir.SetInverse();
        if (inv_dir.x >= 0.0f)
        {
            t_min = (box_min.x - ray.position.x) * inv_dir.x;
            t_max = (box_max.x - ray.position.x) * inv_dir.x;
        }
        else
        {
            t_min = (box_max.x - ray.position.x) * inv_dir.x;
            t_max = (box_min.x - ray.position.x) * inv_dir.x;
        }
        if (inv_dir.y >= 0.0f)
        {
            t_y_min = (box_min.y - ray.position.y) * inv_dir.y;
            t_y_max = (box_max.y - ray.position.y) * inv_dir.y;
        }
        else
        {
            t_y_min = (box_max.y - ray.position.y) * inv_dir.y;
            t_y_max = (box_min.y - ray.position.y) * inv_dir.y;
        }
        if ((t_min > t_y_max) || (t_y_min > t_max))
        {
            return false;
        }
        if (t_y_min > t_min)
        {
            t_min = t_y_min;
        }
        if (t_y_max < t_max)
        {
            t_max = t_y_max;
        }
        if (inv_dir.z >= 0.0f)
        {
            t_z_min = (box_min.z - ray.position.z) * inv_dir.z;
            t_z_max = (box_max.z - ray.position.z) * inv_dir.z;
        }
        else
        {
            t_z_min = (box_max.z - ray.position.z) * inv_dir.z;
            t_z_max = (box_min.z - ray.position.z) * inv_dir.z;
        }
        if ((t_min > t_z_max) || (t_z_min > t_max))
        {
            return false;
        }
        if (t_z_min > t_min)
        {
            t_min = t_z_min;
        }
        if (t_z_max < t_max)
        {
            t_max = t_z_max;
        }
        t = t_min;
        if (t_min < 0.0f && t_max < 0.0f)
        {
            return false;
        }

        // check max distance.
        if (t > max_distance && max_distance >= 0.0f)
        {
            return false;
        }

        // done, have intersection
        return true;
    }

    bool BoundingAABB::Contains(BoundingAABB* aabb) const
    {
        if (this->Intersect(aabb) == true)
        {
            if (m_min.x > aabb->m_min.x || aabb->m_max.x > m_max.x)
            {
                return false;
            }
            if (m_min.y > aabb->m_min.y || aabb->m_max.y > m_max.y)
            {
                return false;
            }
            if (m_min.z > aabb->m_min.z || aabb->m_max.z > m_max.z)
            {
                return false;
            }
            return true;
        }
        return false;
    }

    Real BoundingAABB::Volume() const
    {
        return (m_max.x - m_min.x) * (m_max.y - m_min.y) * (m_max.z - m_min.z);
    }

    Real BoundingAABB::Growth(const BoundingAABB& aabb) const
    {
        BoundingAABB new_bounding_volume(*this, aabb);
        return new_bounding_volume.HalfSize().LengthSquared() - this->HalfSize().LengthSquared();
    }

    Vector3 BoundingAABB::Center() const
    {
        return 0.5f * (m_max + m_min);
    }

    Vector3 BoundingAABB::Size() const
    {
        return m_max - m_min;
    }

    Vector3 BoundingAABB::HalfSize() const
    {
        return 0.5f * (m_max - m_min);
    }

    BoundingAABB BoundingAABB::Union(const BoundingAABB& aabb) const
    {
        BoundingAABB result;
        result.m_min = this->m_min;
        result.m_max = this->m_max;
        if (aabb.m_min.x < this->m_min.x)
        {
            result.m_min.x = aabb.m_min.x;
        }
        if (aabb.m_min.y < this->m_min.y)
        {
            result.m_min.y = aabb.m_min.y;
        }
        if (aabb.m_min.z < this->m_min.z)
        {
            result.m_min.z = aabb.m_min.z;
        }
        if (aabb.m_max.x > this->m_max.x)
        {
            result.m_max.x = aabb.m_max.x;
        }
        if (aabb.m_max.y > this->m_max.y)
        {
            result.m_max.y = aabb.m_max.y;
        }
        if (aabb.m_max.z > this->m_max.z)
        {
            result.m_max.z = aabb.m_max.z;
        }
        return result;
    }

    ColliderPrimitive* BoundingAABB::GetCollider() const
    {
        return m_collider;
    }

    void* BoundingAABB::GetUserData() const
    {
        return m_userdata;
    }
}
