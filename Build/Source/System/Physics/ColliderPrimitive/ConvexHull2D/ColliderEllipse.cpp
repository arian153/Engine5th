#include "ColliderEllipse.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace Engine5
{
    ColliderEllipse::ColliderEllipse()
    {
    }

    ColliderEllipse::~ColliderEllipse()
    {
    }

    void ColliderEllipse::Initialize()
    {
        SetEllipse(Vector2(0.5f, 0.5f));
    }

    void ColliderEllipse::Shutdown()
    {
    }

    Vector3 ColliderEllipse::Support(const Vector3& direction)
    {
        Vector2 radius = Radius();
        Vector2 sub_space_direction(direction);
        sub_space_direction.SetNormalize();
        Vector2 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(sub_space_direction);
        result /= radius.HadamardProduct(sub_space_direction).Length();
        return Vector3(result.x, result.y, 0.0f);
    }

    bool ColliderEllipse::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector2 radius        = Radius();
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (radius.y * radius.y);
        minimum_t             = -1.0f;
        maximum_t             = -1.0f;
        //Quadratic elements
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x +
                local_ray.direction.y * local_ray.direction.y * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x +
                2.0f * local_ray.direction.y * local_ray.position.y * denominator_y;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x +
                local_ray.position.y * local_ray.position.y * denominator_y - 1.0f;
        Real ellipse_min_t, ellipse_max_t;
        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Utility::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Utility::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                if (Utility::SolveQuadratic(a, b, c, ellipse_max_t, ellipse_min_t))
                {
                    //solve intersection
                    if (Utility::IsEqual(ellipse_min_t, ellipse_max_t))
                    {
                        minimum_t = maximum_t = ellipse_min_t;
                    }
                    else
                    {
                        minimum_t = ellipse_min_t;
                        maximum_t = ellipse_max_t;
                    }
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            Real ellipse_result = plane_intersection.x * plane_intersection.x * denominator_x + plane_intersection.y * plane_intersection.y * denominator_y;
            if (ellipse_result <= 1.0f)
            {
                minimum_t = maximum_t = plane_t;
            }
            else
            {
                return false;
            }
        }
        if (minimum_t < 0.0f && maximum_t < 0.0f)
        {
            return false;
        }
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        return true;
    }

    Vector3 ColliderEllipse::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector2 radius         = Radius();
        Real    ellipse_result = local_point_on_collider.x * local_point_on_collider.x / (radius.x * radius.x)
                + local_point_on_collider.y * local_point_on_collider.y / (radius.y * radius.y);
        if (Utility::IsEqual(ellipse_result, 1.0f))
        {
            Vector3 normal;
            normal.x = 2.0f * local_point_on_collider.x / radius.x * radius.x;
            normal.y = 2.0f * local_point_on_collider.y / radius.y * radius.y;
            normal.SetNormalize();
            return normal;
        }
        return Math::Vector3::Z_AXIS;
    }

    void ColliderEllipse::SetMassData(Real density)
    {
        Real a, b;
        m_density = density;
        m_mass    = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.y;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.y;
        }
        Real it_xx = 0.25f * m_mass * b * b;
        Real it_yy = 0.25f * m_mass * a * a;
        Real it_zz = 0.25f * m_mass * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Math::Vector3::ORIGIN;
    }

    Real ColliderEllipse::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_scaled_radius.x * m_scaled_radius.y;
        }
        return Math::PI * m_radius.x * m_radius.y;
    }

    void ColliderEllipse::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius.HadamardProduct(Vector2(scale));
        m_scale_factor  = scale.Length();
    }

    void ColliderEllipse::SetUnit()
    {
        m_radius.SetNormalize();
        UpdatePrimitive();
    }

    void ColliderEllipse::UpdateBoundingVolume()
    {
        Real    bounding_factor = m_radius.Length();
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

    void ColliderEllipse::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = renderer->CIRCULAR_VERTICES_COUNT;
        renderer->ReserveVertices(count, mode);
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        Vector2    radius           = Radius();
        Real       radian_step      = Math::TWO_PI / static_cast<Real>(count);
        for (int i = 0; i < count; ++i)
        {
            Real    angle = static_cast<Real>(i) * radian_step;
            Vector3 vertex(cosf(angle) * radius.x, sinf(angle) * radius.y, 0.0f);
            vertex = m_orientation.Rotate(vertex);
            vertex += m_position;
            vertex = body_orientation.Rotate(vertex);
            vertex += body_position;
            renderer->PushVertex(vertex, mode, color);
        }
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            renderer->ReserveIndices(200, mode);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushLineIndices(index + i, index + i + 1);
            }
            renderer->PushLineIndices(index + count - 1, index);
        }
        else if (mode == eRenderingMode::Face)
        {
            //add a center pos
            I32     center   = static_cast<I32>(renderer->VerticesSize(mode));
            Vector3 position = m_position;
            position         = body_orientation.Rotate(position);
            position += body_position;
            renderer->PushVertex(position, mode, color);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushFaceIndices(center, index + i + 1, index + i);
            }
            renderer->PushFaceIndices(center, index, index + count - 1);
        }
    }

    Vector2 ColliderEllipse::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderEllipse::SetEllipse(const Vector2& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderEllipse::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderEllipse* ellipse = static_cast<ColliderEllipse*>(origin);
            //collider local space data
            m_orientation  = ellipse->m_orientation;
            m_position     = ellipse->m_position;
            m_scale_factor = ellipse->m_scale_factor;
            //collider mass data
            m_centroid             = ellipse->m_centroid;
            m_mass                 = ellipse->m_mass;
            m_local_inertia_tensor = ellipse->m_local_inertia_tensor;
            m_density              = ellipse->m_density;
            m_material             = ellipse->m_material;
            //ellipse
            m_radius        = ellipse->m_radius;
            m_scaled_radius = ellipse->m_scaled_radius;
        }
    }

    void ColliderEllipse::Load(const Json::Value& data)
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
        if (JsonResource::HasMember(data, "Radius") && JsonResource::IsVector2(data["Radius"]))
        {
            m_radius        = JsonResource::AsVector2(data["Radius"]);
            m_scaled_radius = m_scale_factor * m_radius;
        }
    }

    void ColliderEllipse::Save(const Json::Value& data)
    {
    }
}
