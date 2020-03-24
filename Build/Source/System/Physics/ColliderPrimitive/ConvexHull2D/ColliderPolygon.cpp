#include "ColliderPolygon.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace Engine5
{
    ColliderPolygon::ColliderPolygon()
    {
    }

    ColliderPolygon::~ColliderPolygon()
    {
    }

    void ColliderPolygon::Initialize()
    {
    }

    void ColliderPolygon::Shutdown()
    {
    }

    Vector3 ColliderPolygon::Support(const Vector3& direction)
    {
        Real                  p = Math::REAL_NEGATIVE_MAX;
        Vector3               result;
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            Real projection = Vector3(vertices->at(i)).DotProduct(direction);
            if (projection > p)
            {
                result = vertices->at(i);
                p      = projection;
            }
        }
        return result;
    }

    bool ColliderPolygon::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //Quadratic elements
        Real polygon_min_t = Math::REAL_POSITIVE_MAX;
        Real polygon_max_t = Math::REAL_NEGATIVE_MAX;
        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Utility::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Utility::IsZero(pc.DotProduct(normal)) == true)
            {
                std::vector<Vector2>* vertices;
                if (m_collider_set != nullptr)
                {
                    vertices = m_scaled_vertices;
                }
                else
                {
                    vertices = m_vertices;
                }
                //ray is on the plane.
                Vector2 dir(local_ray.direction.x, local_ray.direction.y);
                Vector2 pos(local_ray.position.x, local_ray.position.y);
                Real    inv_dir = 1.0f / dir.DotProduct(dir);
                size_t  size    = vertices->size();
                for (size_t i = 0; i < size; ++i)
                {
                    size_t  j    = i + 1 < size ? i + 1 : 0;
                    Vector2 p0   = vertices->at(i);
                    Vector2 p1   = vertices->at(j);
                    Vector2 edge = p1 - p0;
                    if (Utility::IsZero(dir.CrossProduct(edge)) == true)
                    {
                        if (Utility::IsZero((p0 - pos).CrossProduct(dir)) == true)
                        {
                            polygon_min_t = (p0 - pos).DotProduct(dir) * inv_dir;
                            polygon_max_t = (p1 - pos).DotProduct(dir) * inv_dir;
                            break;
                        }
                    }
                    else
                    {
                        Real t = (p0 - pos).CrossProduct(edge) / (dir.CrossProduct(edge));
                        if (polygon_min_t > t)
                        {
                            polygon_min_t = t;
                        }
                        if (polygon_max_t < t)
                        {
                            polygon_max_t = t;
                        }
                    }
                }
                //find min max t.
                minimum_t = polygon_min_t;
                maximum_t = polygon_max_t;
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
            bool is_rectangle_contain_point = false;
            if (is_rectangle_contain_point)
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

    Vector3 ColliderPolygon::GetNormal(const Vector3& local_point_on_collider)
    {
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t  size = vertices->size();
        Vector2 v(local_point_on_collider);
        for (size_t i = 0; i < size; ++i)
        {
            size_t  j    = i + 1 < size ? i + 1 : 0;
            Vector2 p0   = vertices->at(i);
            Vector2 p1   = vertices->at(j);
            Vector2 edge = p1 - p0;
            if (p0.IsEqual(p1) == false)
            {
                Real tx = (v.x - p0.x) / edge.x;
                Real ty = (v.y - p0.y) / edge.y;
                Real t  = Utility::IsEqual(tx, ty) ? tx : (!Utility::IsZero(tx) ? tx : ty);
                //is point on edge ?
                if (t <= 1.0f && t >= 0.0f)
                {
                    return Vector3(-v.y, v.x);
                }
            }
        }
        return Math::Vector3::Z_AXIS;
    }

    void ColliderPolygon::SetMassData(Real density)
    {
        Real area  = 0.0f;
        Real it_xx = 0.0f;
        Real it_yy = 0.0f;
        Real it_zz = 0.0f;
        Real inv3  = 1.0f / 3.0f;
        //vertices
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  j          = i + 1 < size ? i + 1 : 0;
            p1                 = Vector3(vertices->at(i).x, vertices->at(i).y);
            p2                 = Vector3(vertices->at(j).x, vertices->at(j).y);
            Real triangle_area = 0.5f * p1.CrossProduct(p2).Length();
            area += triangle_area;
            // Use area to weight the centroid average, not just vertex position
            m_centroid += (p1 + p2) * triangle_area * inv3;
            Real it_x = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
            Real it_y = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
            it_xx += 0.25f * inv3 * p1.CrossProduct(p2).Length() * it_y;
            it_yy += 0.25f * inv3 * p1.CrossProduct(p2).Length() * it_x;
            it_zz += 0.25f * inv3 * p1.CrossProduct(p2).Length() * (it_x + it_y);
        }
        m_centroid *= 1.0f / area;
        m_mass = density * area;
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx * density, it_yy * density, it_zz * density);
    }

    Real ColliderPolygon::GetVolume()
    {
        Real area = 0.0f;
        //vertices
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  j = i + 1 < size ? i + 1 : 0;
            p1        = Vector3(vertices->at(i).x, vertices->at(i).y);
            p2        = Vector3(vertices->at(j).x, vertices->at(j).y);
            //area
            Real triangle_area = 0.5f * p1.CrossProduct(p2).Length();
            area += triangle_area;
        }
        return area;
    }

    void ColliderPolygon::SetScaleData(const Vector3& scale)
    {
        size_t  size = m_vertices->size();
        Vector2 scale_v2(scale.x, scale.y);
        for (size_t i = 0; i < size; ++i)
        {
            m_scaled_vertices->at(i) = scale_v2.HadamardProduct(m_vertices->at(i));
        }
        m_scale_factor = scale.Length();
    }

    void ColliderPolygon::SetUnit()
    {
        auto min_max     = m_max_bound - m_min_bound;
        Real unit_factor = 1.0f / min_max.Length();
        for (auto& vertex : *m_vertices)
        {
            vertex *= unit_factor;
        }
        UpdatePrimitive();
    }

    void ColliderPolygon::UpdateBoundingVolume()
    {
        Real    bounding_factor = (m_max_bound - m_min_bound).Length() * 0.5f;
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

    void ColliderPolygon::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32                   index = static_cast<I32>(renderer->VerticesSize(mode));
        std::vector<Vector2>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        renderer->ReserveVertices(size, mode);
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();
        for (auto& vertex : *vertices)
        {
            //collider local space to object space(body local)
            Vector3 vertex_v3(vertex);
            vertex_v3 = m_orientation.Rotate(vertex_v3);
            vertex_v3 += m_position;
            //body local space to world space
            vertex_v3 = body_orientation.Rotate(vertex_v3);
            vertex_v3 += body_position;
            //push to renderer
            renderer->PushVertex(vertex_v3, mode, color);
        }
        //add indices
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < size; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (auto& edge : *m_edges)
            {
                renderer->PushLineIndices(index + (I32)edge.a, index + (I32)edge.b);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            Vector3 vertex_v3 = Math::Vector3::ORIGIN;
            vertex_v3         = m_orientation.Rotate(vertex_v3);
            vertex_v3 += m_position;
            vertex_v3 = body_orientation.Rotate(vertex_v3);
            vertex_v3 += body_position;
            renderer->PushVertex(vertex_v3, mode, color);
            for (auto& edge : *m_edges)
            {
                renderer->PushFaceIndices(index + (I32)edge.a, index + (I32)edge.b, (I32)size);
            }
        }
    }

    Vector2 ColliderPolygon::Vertex(size_t i) const
    {
        if (i >= m_vertices->size())
        {
            i = m_vertices->size() - 1;
        }
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices->at(i);
        }
        return m_vertices->at(i);
    }

    bool ColliderPolygon::SetPolygon(const std::vector<Vector2>& vertices)
    {
        size_t size = vertices.size();
        if (size < 3)
        {
            return false;
        }
        return true;
    }

    void ColliderPolygon::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderPolygon* polygon = static_cast<ColliderPolygon*>(origin);
            //collider local space data
            m_orientation  = polygon->m_orientation;
            m_position     = polygon->m_position;
            m_scale_factor = polygon->m_scale_factor;
            //collider mass data
            m_centroid             = polygon->m_centroid;
            m_mass                 = polygon->m_mass;
            m_local_inertia_tensor = polygon->m_local_inertia_tensor;
            m_density              = polygon->m_density;
            m_material             = polygon->m_material;
            //polygon
            *m_vertices        = *polygon->m_vertices;
            *m_scaled_vertices = *polygon->m_scaled_vertices;
            *m_edges           = *polygon->m_edges;
            m_min_bound        = polygon->m_min_bound;
            m_max_bound        = polygon->m_max_bound;
        }
    }

    void ColliderPolygon::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Orientation") && JsonResource::IsQuaternion(data[ "Orientation" ]))
        {
            m_orientation = JsonResource::AsQuaternionRIJK(data[ "Orientation" ]);
        }
        if (JsonResource::HasMember(data, "Position") && JsonResource::IsVector3(data[ "Position" ]))
        {
            m_position = JsonResource::AsVector3(data[ "Position" ]);
        }
        if (JsonResource::HasMember(data, "Scale") && data[ "Scale" ].isDouble())
        {
            m_scale_factor = data[ "Scale" ].asFloat();
        }
        if (JsonResource::HasMember(data, "Centroid") && JsonResource::IsVector3(data[ "Centroid" ]))
        {
            m_centroid = JsonResource::AsVector3(data[ "Centroid" ]);
        }
        if (JsonResource::HasMember(data, "Mass") && data[ "Mass" ].isDouble())
        {
            m_mass = data[ "Mass" ].asFloat();
        }
        if (JsonResource::HasMember(data, "Inertia") && JsonResource::IsMatrix33(data[ "Inertia" ]))
        {
            m_local_inertia_tensor = JsonResource::AsMatrix33(data[ "Inertia" ]);
        }
        if (JsonResource::HasMember(data, "Density") && data[ "Density" ].isDouble())
        {
            m_density = data[ "Density" ].asFloat();
        }
        if (JsonResource::HasMember(data, "Material") && data[ "Material" ].isString())
        {
            std::string material = data[ "Material" ].asString();
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
    }

    void ColliderPolygon::Save(const Json::Value& data)
    {
    }

    void ColliderPolygon::CreateSimplex()
    {
    }

    void ColliderPolygon::AddToOutsideSet()
    {
    }

    void ColliderPolygon::CalculateHorizon()
    {
    }
}
