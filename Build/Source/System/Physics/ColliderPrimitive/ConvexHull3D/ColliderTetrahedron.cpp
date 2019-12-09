#include "ColliderTetrahedron.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

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
        SetTetrahedron(Vector3::Origin(), Vector3::AxisX(), Vector3::AxisY(), Vector3::AxisZ());
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
        m_density  = density;
        m_centroid = 0.0f;
        m_local_inertia_tensor.SetZero();
        m_mass        = 0.0f;
        Vector3  ref  = Vertex(0);
        Vector3  v1   = Vertex(1);
        Vector3  v2   = Vertex(2);
        Vector3  v3   = Vertex(3);
        Real     axis = 1.0f / 60.0f;
        Real     prod = 1.0f / 120.0f;
        Matrix33 canonical_matrix(axis, prod, prod, prod, axis, prod, prod, prod, axis);
        Matrix33 transformation_matrix;
        transformation_matrix.SetColumns(v1 - ref, v2 - ref, v3 - ref);

        //calculate sub inertia
        m_local_inertia_tensor = transformation_matrix.Determinant() * transformation_matrix * canonical_matrix * transformation_matrix.Transpose();

        //volume is 1 / 6 of triple product, that is 1/6 det of transformation matrix.
        m_mass = density * transformation_matrix.Determinant() / 6.0f;

        //The center-of-mass is just the mean of the four vertex coordinates. 
        m_centroid = (ref + v1 + v2 + v3) * 0.25f;
        if (ref.IsZero() == false)
        {
            m_local_inertia_tensor += m_mass * (ref.OuterProduct(m_centroid) + m_centroid.OuterProduct(ref) + ref.OuterProduct(ref));
        }
    }

    Real ColliderTetrahedron::GetVolume()
    {
        Vector3  ref = Vertex(0);
        Vector3  v1  = Vertex(1);
        Vector3  v2  = Vertex(2);
        Vector3  v3  = Vertex(3);
        Matrix33 transformation_matrix;
        transformation_matrix.SetColumns(v1 - ref, v2 - ref, v3 - ref);
        return transformation_matrix.Determinant() / 6.0f;
    }

    void ColliderTetrahedron::SetScaleData(const Vector3& scale)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            m_scaled_vertices[i] = m_vertices[i].HadamardProduct(scale);
        }
        m_scale_factor = scale.Length();
    }

    void ColliderTetrahedron::SetUnit()
    {
        Vector3 max_bound, min_bound;
        max_bound = Math::REAL_NEGATIVE_MAX;
        min_bound = Math::REAL_POSITIVE_MAX;
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                if (max_bound[j] < m_vertices[i][j])
                {
                    max_bound[j] = m_vertices[i][j];
                }
                if (min_bound[j] > m_vertices[i][j])
                {
                    min_bound[j] = m_vertices[i][j];
                }
            }
        }
        auto scale_factor = (max_bound - min_bound).Inverse();
        for (size_t i = 0; i < 4; ++i)
        {
            m_vertices[i] = m_vertices[i].HadamardProduct(scale_factor);
        }
        UpdatePrimitive();
    }

    void ColliderTetrahedron::UpdateBoundingVolume()
    {
        Vector3 max_bound, min_bound;
        max_bound               = Math::REAL_NEGATIVE_MAX;
        min_bound               = Math::REAL_POSITIVE_MAX;
        Real    bounding_factor = (max_bound - min_bound).Length();
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

    void ColliderTetrahedron::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(4, mode);
        Vector3 vertices[4];
        if (m_collider_set != nullptr)
        {
            std::memcpy(vertices, m_scaled_vertices, sizeof(m_scaled_vertices));
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
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices[i];
        }
        return m_vertices[i];
    }

    void ColliderTetrahedron::SetTetrahedron(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        m_vertices[0] = v0;
        m_vertices[1] = v1;
        m_vertices[2] = v2;
        m_vertices[3] = v3;
        UpdatePrimitive();
    }

    void ColliderTetrahedron::Clone(ColliderPrimitive* cloned)
    {
    }
}
