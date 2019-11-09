#include "ColliderBox.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    ColliderBox::ColliderBox()
    {
    }

    ColliderBox::~ColliderBox()
    {
    }

    Vector3 ColliderBox::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Real    p         = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 8; ++i)
        {
            Real projection = Vertex(i).DotProduct(local_dir);
            if (projection > p)
            {
                result = Vertex(i);
                p      = projection;
            }
        }
        return LocalToWorldPoint(result);
    }

    bool ColliderBox::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;
        Vector3 box_min = Vertex(7);
        Vector3 box_max = Vertex(0);
        Vector3 inv_dir = local_ray.direction;
        inv_dir.SetInverse();
        if (inv_dir.x >= 0.0f)
        {
            t_min = (box_min.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_max.x - local_ray.position.x) * inv_dir.x;
        }
        else
        {
            t_min = (box_max.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_min.x - local_ray.position.x) * inv_dir.x;
        }
        if (inv_dir.y >= 0.0f)
        {
            t_y_min = (box_min.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_max.y - local_ray.position.y) * inv_dir.y;
        }
        else
        {
            t_y_min = (box_max.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_min.y - local_ray.position.y) * inv_dir.y;
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
            t_z_min = (box_min.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_max.z - local_ray.position.z) * inv_dir.z;
        }
        else
        {
            t_z_min = (box_max.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_min.z - local_ray.position.z) * inv_dir.z;
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
        if (t_min < 0.0f && t_max < 0.0f)
        {
            return false;
        }
        minimum_t = t_min;
        maximum_t = t_max;
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        return true;
    }

    Vector3 ColliderBox::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        Real    w = (Vertex(0) - Vertex(4)).x;
        Real    h = (Vertex(0) - Vertex(2)).y;
        Real    d = (Vertex(0) - Vertex(1)).z;
        Vector3 size(w, h, d);
        Real    min      = Math::REAL_POSITIVE_MAX;
        Real    distance = fabsf(size.x - fabsf(local_point_on_collider.x));
        if (distance < min)
        {
            min = distance;
            normal.Set(1.0f, 0.0f, 0.0f);
            normal *= Utility::Signum(local_point_on_collider.x);
        }
        distance = fabsf(size.y - fabsf(local_point_on_collider.y));
        if (distance < min)
        {
            min = distance;
            normal.Set(0.0f, 1.0f, 0.0f);
            normal *= Utility::Signum(local_point_on_collider.y);
        }
        distance = fabsf(size.z - fabsf(local_point_on_collider.z));
        if (distance < min)
        {
            normal.Set(0.0f, 0.0f, 1.0f);
            normal *= Utility::Signum(local_point_on_collider.z);
        }
        return normal;
    }

    void ColliderBox::CalculateMassData(Real density)
    {
        Real w, h, d;
        if (m_rigid_body != nullptr)
        {
            w = (m_transformed_vertices[0] - m_transformed_vertices[4]).x;
            h = (m_transformed_vertices[0] - m_transformed_vertices[2]).y;
            d = (m_transformed_vertices[0] - m_transformed_vertices[1]).z;
        }
        else
        {
            w = (m_vertices[0] - m_vertices[4]).x;
            h = (m_vertices[0] - m_vertices[2]).y;
            d = (m_vertices[0] - m_vertices[1]).z;
        }
        m_mass    = density * w * h * d;
        Real it_a = m_mass / 12.0f * (h * h + d * d);
        Real it_b = m_mass / 12.0f * (w * w + d * d);
        Real it_c = m_mass / 12.0f * (w * w + h * h);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_a, it_b, it_c);
        m_centroid = Vector3(0.5f * w, 0.5f * h, 0.5f * d) + Vertex(7);
    }

    Real ColliderBox::GetVolume()
    {
        if (m_rigid_body != nullptr)
        {
            Real w = (m_transformed_vertices[0] - m_transformed_vertices[4]).x;
            Real h = (m_transformed_vertices[0] - m_transformed_vertices[2]).y;
            Real d = (m_transformed_vertices[0] - m_transformed_vertices[1]).z;
            return w * h * d;
        }
        Real w = (m_vertices[0] - m_vertices[4]).x;
        Real h = (m_vertices[0] - m_vertices[2]).y;
        Real d = (m_vertices[0] - m_vertices[1]).z;
        return w * h * d;
    }

    void ColliderBox::ScalePrimitiveData(const Vector3& scale)
    {
        for (size_t i = 0; i < 8; ++i)
        {
            m_transformed_vertices[i] = m_vertices[i].HadamardProduct(scale);
        }
    }

    void ColliderBox::MakeUnitPrimitive()
    {
        Real w = 0.5f;
        Real h = 0.5f;
        Real d = 0.5f;
        m_vertices[0].Set(+w, +h, +d);
        m_vertices[1].Set(+w, +h, -d);
        m_vertices[2].Set(+w, -h, +d);
        m_vertices[3].Set(+w, -h, -d);
        m_vertices[4].Set(-w, +h, +d);
        m_vertices[5].Set(-w, +h, -d);
        m_vertices[6].Set(-w, -h, +d);
        m_vertices[7].Set(-w, -h, -d);

        //TODO - get scale from transform 
        Vector3 scale(1.0f, 1.0f, 1.0f);
        ScalePrimitiveData(scale);
    }

    void ColliderBox::UpdateBoundingVolume()
    {
        //Vector3 min = Vertex(7);
        //Vector3 max = Vertex(0);
        //m_bounding_volume->Set();
    }

    void ColliderBox::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(8, mode);
        Vector3 vertices[8];
        if (m_rigid_body != nullptr)
        {
            std::memcpy(vertices, m_transformed_vertices, sizeof(m_transformed_vertices));
        }
        else
        {
            std::memcpy(vertices, m_vertices, sizeof(m_vertices));
        }
        Vector3    body_position    = GetBodyPosition();
        Quaternion body_orientation = GetBodyOrientation();

        for (auto& vertex : vertices)
        {
            //collider local space to object space(body local)
            vertex = m_orientation.Rotate(vertex);
            vertex += m_position;

            //body local space to world space
            vertex = body_orientation.Rotate(vertex);
            vertex += body_position;
            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }

        //add indices
        if (mode == RenderingMode::Dot)
        {
            for (I32 i = 0; i < 8; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            //front
            renderer->PushLineIndices(index + 0, index + 2);
            renderer->PushLineIndices(index + 2, index + 6);
            renderer->PushLineIndices(index + 6, index + 4);
            renderer->PushLineIndices(index + 4, index + 0);
            //back
            renderer->PushLineIndices(index + 1, index + 3);
            renderer->PushLineIndices(index + 3, index + 7);
            renderer->PushLineIndices(index + 7, index + 5);
            renderer->PushLineIndices(index + 5, index + 1);
            //side
            renderer->PushLineIndices(index + 0, index + 1);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 6, index + 7);
            renderer->PushLineIndices(index + 4, index + 5);
        }
        else if (mode == RenderingMode::Face)
        {
            //front
            renderer->PushFaceIndices(index + 0, index + 2, index + 4);
            renderer->PushFaceIndices(index + 2, index + 6, index + 4);
            //back
            renderer->PushFaceIndices(index + 1, index + 3, index + 5);
            renderer->PushFaceIndices(index + 3, index + 7, index + 5);
            //right
            renderer->PushFaceIndices(index + 0, index + 1, index + 3);
            renderer->PushFaceIndices(index + 2, index + 3, index + 0);
            //left
            renderer->PushFaceIndices(index + 6, index + 7, index + 5);
            renderer->PushFaceIndices(index + 4, index + 5, index + 6);
            //top
            renderer->PushFaceIndices(index + 1, index + 0, index + 4);
            renderer->PushFaceIndices(index + 1, index + 4, index + 5);
            //bottom
            renderer->PushFaceIndices(index + 3, index + 2, index + 6);
            renderer->PushFaceIndices(index + 3, index + 6, index + 7);
        }
    }

    Vector3 ColliderBox::Vertex(size_t i) const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_vertices[i];
        }
        return m_vertices[i];
    }

    void ColliderBox::Clone(ColliderPrimitive* cloned)
    {
    }
}
