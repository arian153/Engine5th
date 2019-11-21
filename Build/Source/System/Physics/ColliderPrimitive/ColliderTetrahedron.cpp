#include "ColliderTetrahedron.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    ColliderTetrahedron::ColliderTetrahedron()
    {
        m_type = ColliderType::Tetrahedron;
    }

    ColliderTetrahedron::~ColliderTetrahedron()
    {
    }

    void ColliderTetrahedron::Initialize()
    {
    }

    void ColliderTetrahedron::Shutdown()
    {
    }

    Vector3 ColliderTetrahedron::Support(const Vector3& direction)
    {
        Vector3 local_dir = WorldToLocalVector(direction).Unit();
        Real    p         = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 4; ++i)
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

    bool ColliderTetrahedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 ColliderTetrahedron::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderTetrahedron::SetMassData(Real density)
    {
    }

    Real ColliderTetrahedron::GetVolume()
    {
        return m_mass;
    }

    void ColliderTetrahedron::UpdateScale(const Vector3& scale)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            m_transformed_vertices[i] = m_vertices[i].HadamardProduct(scale);
        }
        m_scale_factor = scale.Length();
    }

    void ColliderTetrahedron::SetUnit()
    {
        m_vertices[0] = Vector3::Origin();
        m_vertices[1] = Vector3::AxisX();
        m_vertices[2] = Vector3::AxisY();
        m_vertices[3] = Vector3::AxisZ();
        if (m_collider_set != nullptr)
        {
            UpdateScale(m_collider_set->GetTransformScale());
            m_collider_set->CalculateMassData();
        }
    }

    void ColliderTetrahedron::UpdateBoundingVolume()
    {
    }

    void ColliderTetrahedron::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(4, mode);
        Vector3 vertices[4];
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
            for (I32 i = 0; i < 4; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            renderer->PushLineIndices(index + 0, index + 1);
            renderer->PushLineIndices(index + 1, index + 2);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 3, index + 0);
        }
        else if (mode == RenderingMode::Face)
        {
            renderer->PushFaceIndices(index + 0, index + 1, index + 2);
            renderer->PushFaceIndices(index + 0, index + 1, index + 3);
            renderer->PushFaceIndices(index + 0, index + 2, index + 3);
            renderer->PushFaceIndices(index + 1, index + 2, index + 3);
        }
    }

    Vector3 ColliderTetrahedron::Vertex(size_t i) const
    {
        if (m_rigid_body != nullptr)
        {
            return m_transformed_vertices[i];
        }
        return m_vertices[i];
    }

    void ColliderTetrahedron::SetTetrahedron(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        m_vertices[0] = v0;
        m_vertices[1] = v1;
        m_vertices[2] = v2;
        m_vertices[3] = v3;
        if (m_collider_set != nullptr)
        {
            UpdateScale(m_collider_set->GetTransformScale());
            m_collider_set->CalculateMassData();
        }
    }

    void ColliderTetrahedron::Clone(ColliderPrimitive* cloned)
    {
    }
}
