#include "ColliderSphere.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace Engine5
{
    ColliderSphere::ColliderSphere()
    {
        m_type = eColliderType::Sphere;
    }

    ColliderSphere::~ColliderSphere()
    {
    }

    void ColliderSphere::Initialize()
    {
        SetSphere(0.5f);
    }

    void ColliderSphere::Shutdown()
    {
    }

    Vector3 ColliderSphere::Support(const Vector3& direction)
    {
        return Radius() * direction;
    }

    bool ColliderSphere::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    radius = Radius();
        Vector3 l      = -local_ray.position;
        Real    tc     = l.DotProduct(local_ray.direction);
        if (tc < 0.0f)
        {
            return false;
        }
        Real d2             = tc * tc - l.DotProduct(l);
        Real radius_squared = radius * radius;
        if (d2 > radius_squared)
        {
            return false;
        }
        //solve for t1c
        Real t1c  = sqrtf(radius_squared - d2);
        minimum_t = tc - t1c;
        maximum_t = tc + t1c;
        return true;
    }

    Vector3 ColliderSphere::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider.Normalize();
    }

    void ColliderSphere::SetMassData(Real density)
    {
        m_mass    = density * GetVolume();
        m_density = density;
        Real it   = m_mass * 0.4f * Radius() * Radius();
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it, it, it);
        m_centroid.SetZero();
    }

    Real ColliderSphere::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return 4.0f / 3.0f * Math::PI * m_scaled_radius * m_scaled_radius * m_scaled_radius;
        }
        return 4.0f / 3.0f * Math::PI * m_radius * m_radius * m_radius;
    }

    void ColliderSphere::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius * scale.Length();
        m_scale_factor  = scale.Length();
    }

    void ColliderSphere::SetUnit()
    {
        m_radius = 0.5f;
        UpdatePrimitive();
    }

    void ColliderSphere::UpdateBoundingVolume()
    {
        Real    bounding_factor = m_radius;
        Vector3 pos;
        if (m_rigid_body != nullptr)
        {
            pos = m_rigid_body->LocalToWorldPoint(m_position);
            bounding_factor *= m_scale_factor;
        }
        else
        {
            pos = m_position;
        }
        Vector3 min_max(bounding_factor, bounding_factor, bounding_factor);
        m_bounding_volume->Set(-min_max + pos, min_max + pos);
    }

    void ColliderSphere::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3    axis_vector      = Math::Vector3::Y_AXIS;
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Real       radius           = Radius();
        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos * radius;
        //modify rotation, translation
        top_vertex_local_pos = m_orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += m_position;
        top_vertex_local_pos = body_orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += body_position;
        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos * radius;
        //modify rotation, translation
        bottom_vertex_local_pos = m_orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += m_position;
        bottom_vertex_local_pos = body_orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += body_position;
        renderer->PushVertex(top_vertex_local_pos, mode, color);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;
                // spherical to cartesian
                Vector3 vertex_local_pos;
                vertex_local_pos.x = sinf(phi) * cosf(theta);
                vertex_local_pos.y = cosf(phi);
                vertex_local_pos.z = sinf(phi) * sinf(theta);
                vertex_local_pos   = vertex_local_pos * radius;
                vertex_local_pos   = m_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += m_position;
                vertex_local_pos = body_orientation.Rotate(vertex_local_pos);
                vertex_local_pos += body_position;
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        renderer->PushVertex(bottom_vertex_local_pos, mode, color);
        if (mode == eRenderingMode::Dot)
        {
            I32 count = renderer->SPHERICAL_VERTICES_COUNT;
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushLineIndices(index, index + i + 1);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(base + i * ring + j, base + i * ring + j + 1);
                    renderer->PushLineIndices(base + i * ring + j, base + (i + 1) * ring + j);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushLineIndices(south_pole_index, base + i);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushFaceIndices(index, index + i + 1, index + i);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushFaceIndices(
                                              base + i * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j);
                    renderer->PushFaceIndices(
                                              base + (i + 1) * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j + 1);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushFaceIndices(south_pole_index, base + i, base + i + 1);
            }
        }
    }

    Real ColliderSphere::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderSphere::SetSphere(Real radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderSphere::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderSphere* sphere = static_cast<ColliderSphere*>(origin);
            //collider local space data
            m_orientation  = sphere->m_orientation;
            m_position     = sphere->m_position;
            m_scale_factor = sphere->m_scale_factor;
            //collider mass data
            m_centroid             = sphere->m_centroid;
            m_mass                 = sphere->m_mass;
            m_local_inertia_tensor = sphere->m_local_inertia_tensor;
            m_density              = sphere->m_density;
            m_material             = sphere->m_material;
            //sphere
            m_radius        = sphere->m_radius;
            m_scaled_radius = sphere->m_scaled_radius;
        }
    }

    void ColliderSphere::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Orientation") && JsonResource::IsQuaternion(data["Orientation"]))
        {
            m_orientation = JsonResource::AsQuaternionRIJK(data["Orientation"]);
        }
        if (JsonResource::HasMember(data, "Position") && JsonResource::IsVector3(data["Position"]))
        {
            m_position = JsonResource::AsVector3(data["Position"]);
        }
        if (JsonResource::HasMember(data, "Scale") && data["Scale"].isDouble())
        {
            m_scale_factor = data["Scale"].asFloat();
        }
        if (JsonResource::HasMember(data, "Centroid") && JsonResource::IsVector3(data["Centroid"]))
        {
            m_centroid = JsonResource::AsVector3(data["Centroid"]);
        }
        if (JsonResource::HasMember(data, "Mass") && data["Mass"].isDouble())
        {
            m_mass = data["Mass"].asFloat();
        }
        if (JsonResource::HasMember(data, "Inertia") && JsonResource::IsMatrix33(data["Inertia"]))
        {
            m_local_inertia_tensor = JsonResource::AsMatrix33(data["Inertia"]);
        }
        if (JsonResource::HasMember(data, "Density") && data["Density"].isDouble())
        {
            m_density = data["Density"].asFloat();
        }
        if (JsonResource::HasMember(data, "Material") && data["Material"].isString())
        {
            std::string material = data["Material"].asString();
            if (material == "Rock")
            {
                m_material = Physics::eMaterial::Rock;
            }
            else if (material == "Wood")
            {
                m_material = Physics::eMaterial::Wood;
            }
            else if (material == "Metal")
            {
                m_material = Physics::eMaterial::Metal;
            }
            else if (material == "BouncyBall")
            {
                m_material = Physics::eMaterial::BouncyBall;
            }
            else if (material == "SuperBall")
            {
                m_material = Physics::eMaterial::SuperBall;
            }
            else if (material == "Pillow")
            {
                m_material = Physics::eMaterial::Pillow;
            }
            else if (material == "Static")
            {
                m_material = Physics::eMaterial::Static;
            }
            else if (material == "Concrete")
            {
                m_material = Physics::eMaterial::Concrete;
            }
            else if (material == "Ice")
            {
                m_material = Physics::eMaterial::Ice;
            }
            else if (material == "Glass")
            {
                m_material = Physics::eMaterial::Glass;
            }
            else if (material == "Lubricant")
            {
                m_material = Physics::eMaterial::Lubricant;
            }
            else if (material == "Rubber")
            {
                m_material = Physics::eMaterial::Rubber;
            }
            else if (material == "Velcro")
            {
                m_material = Physics::eMaterial::Velcro;
            }
        }
        if (JsonResource::HasMember(data, "Radius") && data["Radius"].isDouble())
        {
            m_radius        = data["Radius"].asFloat();
            m_scaled_radius = m_scale_factor * m_radius;
        }
    }

    void ColliderSphere::Save(const Json::Value& data)
    {
    }
}
