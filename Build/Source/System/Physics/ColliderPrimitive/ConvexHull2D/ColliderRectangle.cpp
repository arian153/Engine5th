#include "ColliderRectangle.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace Engine5
{
    ColliderRectangle::ColliderRectangle()
    {
    }

    ColliderRectangle::~ColliderRectangle()
    {
    }

    void ColliderRectangle::Initialize()
    {
    }

    void ColliderRectangle::Shutdown()
    {
    }

    Vector3 ColliderRectangle::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 4; ++i)
        {
            Real projection = Vector3(Vertex(i)).DotProduct(direction);
            if (projection > p)
            {
                result = Vertex(i);
                p      = projection;
            }
        }
        return result;
    }

    bool ColliderRectangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        return false;
    }

    Vector3 ColliderRectangle::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderRectangle::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real w, h;
        if (m_collider_set != nullptr)
        {
            w = (m_scaled_vertices[0] - m_scaled_vertices[2]).x;
            h = (m_scaled_vertices[0] - m_scaled_vertices[3]).y;
        }
        else
        {
            w = (m_vertices[0] - m_vertices[2]).x;
            h = (m_vertices[0] - m_vertices[3]).y;
        }
        m_mass    = density * w * h;
        Real it_a = m_mass / 12.0f * (h * h);
        Real it_b = m_mass / 12.0f * (w * w);
        Real it_c = m_mass / 12.0f * (w * w + h * h);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_a, it_b, it_c);
        auto vertex = Vertex(7);
        m_centroid  = Vector3(0.5f * w, 0.5f * h, 0.0f) + Vector3(vertex.x, vertex.y, 0.0f);
    }

    Real ColliderRectangle::GetVolume()
    {
        return 0.0f;
    }

    void ColliderRectangle::SetScaleData(const Vector3& scale)
    {
    }

    void ColliderRectangle::SetUnit()
    {
    }

    void ColliderRectangle::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Math::Vector3::ORIGIN, Math::Vector3::ORIGIN);
    }

    void ColliderRectangle::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
    }

    Vector2 ColliderRectangle::Vertex(size_t i) const
    {
        return m_vertices[i];
    }

    void ColliderRectangle::SetRectangle(Real width, Real height)
    {
        Real w = 0.5f * width;
        Real h = 0.5f * height;
        m_vertices[0].Set(+w, +h);
        m_vertices[1].Set(+w, -h);
        m_vertices[2].Set(-w, +h);
        m_vertices[3].Set(-w, -h);
        UpdatePrimitive();
    }

    void ColliderRectangle::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderRectangle* rectangle = static_cast<ColliderRectangle*>(origin);
            //collider local space data
            m_orientation  = rectangle->m_orientation;
            m_position     = rectangle->m_position;
            m_scale_factor = rectangle->m_scale_factor;
            //collider mass data
            m_centroid             = rectangle->m_centroid;
            m_mass                 = rectangle->m_mass;
            m_local_inertia_tensor = rectangle->m_local_inertia_tensor;
            m_material             = rectangle->m_material;
            //rectangle
            std::memcpy(m_vertices, rectangle->m_vertices, sizeof m_vertices);
            std::memcpy(m_scaled_vertices, rectangle->m_scaled_vertices, sizeof m_scaled_vertices);
        }
    }

    void ColliderRectangle::Load(const Json::Value& data)
    {
        LoadTransform(data);
        //rectangle data
        if (JsonResource::HasMember(data, "Vertices") && data["Vertices"].isArray() && data["Vertices"].size() == 4)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (JsonResource::IsVector2(data["Vertices"][i]))
                {
                    m_vertices[i]        = JsonResource::AsVector2(data["Vertices"][i]);
                    m_scaled_vertices[i] = m_scale_factor * m_vertices[i];
                }
            }
        }
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderRectangle::Save(const Json::Value& data)
    {
    }
}
