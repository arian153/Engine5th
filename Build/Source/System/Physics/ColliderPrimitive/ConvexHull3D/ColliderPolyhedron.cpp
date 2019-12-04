#include "ColliderPolyhedron.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"

namespace Engine5
{
    ColliderPolyhedron::ColliderPolyhedron()
    {
        m_type = ColliderType::Polyhedron;
    }

    ColliderPolyhedron::~ColliderPolyhedron()
    {
    }

    void ColliderPolyhedron::Initialize()
    {
        UpdatePrimitive();
    }

    void ColliderPolyhedron::Shutdown()
    {
    }

    Vector3 ColliderPolyhedron::Support(const Vector3& direction)
    {
        return direction;
    }

    bool ColliderPolyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 ColliderPolyhedron::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderPolyhedron::SetMassData(Real density)
    {
        m_density  = density;
        m_centroid = 0.0f;
        m_local_inertia_tensor.SetZero();
        m_mass            = 0.0f;
        Vector3 ref_point = Vector3::Origin();
        for (auto& face : *m_faces)
        {
            auto sub_data = CalculateTetrahedronMassData(ref_point, Vertex(face.a), Vertex(face.b), Vertex(face.c), density);
            m_centroid += sub_data.mass * sub_data.centroid;
            m_mass += sub_data.mass;
            m_local_inertia_tensor += sub_data.inertia;
        }
        if (Utility::IsZero(m_mass) == false)
        {
            m_centroid /= m_mass;
        }
        else
        {
            m_centroid = 0.0f;
        }
        m_local_inertia_tensor = TranslateInertia(m_local_inertia_tensor, m_centroid, m_mass, ref_point - m_centroid);
    }

    Real ColliderPolyhedron::GetVolume()
    {
        Real    volume    = 0.0f;
        Vector3 ref_point = Vector3::Origin();
        for (auto& face : *m_faces)
        {
            Matrix33 tetrahedron_matrix;
            tetrahedron_matrix.SetColumns(Vertex(face.a) - ref_point, Vertex(face.b) - ref_point, Vertex(face.c) - ref_point);
            volume += tetrahedron_matrix.Determinant() / 6.0f;
        }
        return volume;
    }

    void ColliderPolyhedron::SetScaleData(const Vector3& scale)
    {
        m_scale_factor = scale.Length();
    }

    void ColliderPolyhedron::SetUnit()
    {
        UpdatePrimitive();
    }

    void ColliderPolyhedron::UpdateBoundingVolume()
    {
        //todo temporary code
        m_bounding_volume->Set(Vector3::Origin(), Vector3::Origin());
    }

    void ColliderPolyhedron::Draw(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color) const
    {
        I32                   index = static_cast<I32>(renderer->VerticesSize(mode));
        std::vector<Vector3>* vertices;
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
            for (I32 i = 0; i < size; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            for (auto& face : *m_faces)
            {
                renderer->PushLineIndices(index + (I32)face.a, index + (I32)face.b);
                renderer->PushLineIndices(index + (I32)face.b, index + (I32)face.c);
                renderer->PushLineIndices(index + (I32)face.c, index + (I32)face.a);
            }
        }
        else if (mode == RenderingMode::Face)
        {
            for (auto& face : *m_faces)
            {
                renderer->PushFaceIndices(index + (I32)face.a, index + (I32)face.b, index + (I32)face.c);
            }
        }
    }

    Vector3 ColliderPolyhedron::Vertex(size_t i) const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices->at(i);
        }
        return m_vertices->at(i);
    }

    void ColliderPolyhedron::Clone(ColliderPrimitive* cloned)
    {
    }

    Matrix33 ColliderPolyhedron::TranslateInertia(const Matrix33& input, const Vector3& centroid, Real mass, const Vector3& offset) const
    {
        return input + mass * (offset.OuterProduct(centroid) + centroid.OuterProduct(offset) + offset.OuterProduct(offset));
    }

    ColliderPolyhedron::SubMassData ColliderPolyhedron::CalculateTetrahedronMassData(const Vector3& ref, const Vector3& v1, const Vector3& v2, const Vector3& v3, Real density) const
    {
        SubMassData result;
        //canonical moment of inertia data.
        Real     axis = 1.0f / 60.0f;
        Real     prod = 1.0f / 120.0f;
        Matrix33 canonical_matrix(axis, prod, prod, prod, axis, prod, prod, prod, axis);
        Matrix33 transformation_matrix;
        transformation_matrix.SetColumns(v1 - ref, v2 - ref, v3 - ref);

        //calculate sub inertia
        result.inertia = transformation_matrix.Determinant() * transformation_matrix * canonical_matrix * transformation_matrix.Transpose();

        //volume is 1 / 6 of triple product, that is 1/6 det of transformation matrix.
        result.mass = density * transformation_matrix.Determinant() / 6.0f;

        //The center-of-mass is just the mean of the four vertex coordinates. 
        result.centroid = (ref + v1 + v2 + v3) * 0.25f;
        if (ref.IsZero() == false)
        {
            result.inertia = TranslateInertia(result.inertia, result.centroid, result.mass, ref);
        }
        return result;
    }
}
