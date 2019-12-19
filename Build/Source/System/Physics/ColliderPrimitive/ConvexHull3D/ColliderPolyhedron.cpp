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
        if (m_vertices == nullptr)
        {
            m_vertices = new std::vector<Vector3>();
        }
        if (m_scaled_vertices == nullptr)
        {
            m_scaled_vertices = new std::vector<Vector3>();
        }
        if (m_edges == nullptr)
        {
            m_edges = new std::vector<ColliderEdge>();
        }
        if (m_faces == nullptr)
        {
            m_faces = new std::vector<ColliderFace>();
        }
        UpdatePrimitive();
    }

    void ColliderPolyhedron::Shutdown()
    {
        if (m_vertices != nullptr)
        {
            m_vertices->clear();
            delete m_vertices;
            m_vertices = nullptr;
        }
        if (m_scaled_vertices != nullptr)
        {
            m_scaled_vertices->clear();
            delete m_scaled_vertices;
            m_scaled_vertices = nullptr;
        }
        if (m_edges != nullptr)
        {
            m_edges->clear();
            delete m_edges;
            m_edges = nullptr;
        }
        if (m_faces != nullptr)
        {
            m_faces->clear();
            delete m_faces;
            m_faces = nullptr;
        }
    }

    Vector3 ColliderPolyhedron::Support(const Vector3& direction)
    {
        Vector3               local_dir = WorldToLocalVector(direction).Unit();
        Real                  p         = Math::REAL_NEGATIVE_MAX;
        Vector3               result;
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
        for (size_t i = 0; i < size; ++i)
        {
            Real projection = vertices->at(i).DotProduct(local_dir);
            if (projection > p)
            {
                result = vertices->at(i);
                p      = projection;
            }
        }
        return LocalToWorldPoint(result);
    }

    bool ColliderPolyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        bool b_first   = true;
        int  hit_count = 0;
        minimum_t      = -1.0f;
        maximum_t      = -1.0f;
        for (auto& face : *m_faces)
        {
            Real t = -1.0f;
            if (IntersectRayFace(local_ray, face, t) == true)
            {
                if (b_first == true)
                {
                    minimum_t = t;
                    maximum_t = t;
                    b_first   = false;
                }
                else
                {
                    if (t > maximum_t)
                    {
                        maximum_t = t;
                    }
                    if (t < minimum_t)
                    {
                        minimum_t = t;
                    }
                }
                hit_count++;
            }
        }
        if (hit_count > 0)
        {
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
        return false;
    }

    Vector3 ColliderPolyhedron::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        for (auto& face : *m_faces)
        {
            if (this->IsFaceContainPoint(face, local_point_on_collider, normal) == true)
            {
                return normal;
            }
        }
        return normal;
    }

    void ColliderPolyhedron::SetMassData(Real density)
    {
        m_density  = density;
        m_centroid = 0.0f;
        m_local_inertia_tensor.SetZero();
        m_mass            = 0.0f;
        Vector3 ref_point = Math::Vector3::ORIGIN;
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
        Vector3 ref_point = Math::Vector3::ORIGIN;
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
        m_scaled_vertices->clear();
        size_t size = m_vertices->size();
        m_scaled_vertices->reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            m_scaled_vertices->push_back(m_vertices->at(i).HadamardProduct(scale));
            for (size_t j = 0; j < 3; ++j)
            {
                if (m_max_bound[j] < m_vertices->at(i)[j])
                {
                    m_max_bound[j] = m_vertices->at(i)[j];
                }
                if (m_min_bound[j] > m_vertices->at(i)[j])
                {
                    m_min_bound[j] = m_vertices->at(i)[j];
                }
            }
        }
        m_scale_factor = scale.Length();
    }

    void ColliderPolyhedron::SetUnit()
    {
        CalculateMinMaxBound();
        auto   scale_factor = (m_max_bound - m_min_bound).Inverse();
        size_t size         = m_vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            m_vertices->at(i) = m_vertices->at(i).HadamardProduct(scale_factor);
        }
        UpdatePrimitive();
    }

    void ColliderPolyhedron::UpdateBoundingVolume()
    {
        Real    bounding_factor = (m_max_bound - m_min_bound).Length();
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

    size_t ColliderPolyhedron::Size() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices->size();
        }
        return m_vertices->size();
    }

    bool ColliderPolyhedron::SetPolyhedron(const std::vector<Vector3>& vertices)
    {
        size_t size = vertices.size();
        if (size < 4)
        {
            return false;
        }
        return true;
    }

    void ColliderPolyhedron::Clone(ColliderPrimitive* cloned)
    {
    }

    bool ColliderPolyhedron::IntersectRayFace(const Ray& ray, const ColliderFace& face, Real& t) const
    {
        std::vector<Vector3>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        Vector3 edge1 = (*vertices)[face.b] - (*vertices)[face.a];
        Vector3 edge2 = (*vertices)[face.c] - (*vertices)[face.a];
        Vector3 h     = ray.direction.CrossProduct(edge2);
        Real    a     = edge1.DotProduct(h);
        t             = -1.0f;
        if (Utility::IsZero(a))
        {
            return false;
        }
        Real    f = 1.0f / a;
        Vector3 s = ray.position - (*vertices)[face.a];
        Real    u = f * (s.DotProduct(h));
        if (u < 0.0f || u > 1.0f)
        {
            return false;
        }
        Vector3 q = s.CrossProduct(edge1);
        Real    v = f * ray.direction.DotProduct(q);
        if (v < 0.0f || u + v > 1.0f)
        {
            return false;
        }

        // At this stage we can compute t to find out where the intersection point is on the line.
        t = f * edge2.DotProduct(q);
        if (t > Math::EPSILON) // ray intersection
        {
            return true;
        }
        // intersect back side of ray.
        return false;
    }

    bool ColliderPolyhedron::IsFaceContainPoint(const ColliderFace& face, const Vector3& point, Vector3& normal) const
    {
        std::vector<Vector3>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        Vector3 v0     = vertices->at(face.a);
        Vector3 v1     = vertices->at(face.b);
        Vector3 v2     = vertices->at(face.c);
        Vector3 edge01 = v1 - v0;
        Vector3 edge12 = v2 - v1;
        normal         = edge01.CrossProduct(edge12);
        Vector3 w_test = edge01.CrossProduct(point - v0);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        w_test = edge12.CrossProduct(point - v1);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        Vector3 edge3 = v0 - v2;
        w_test        = edge3.CrossProduct(point - v2);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        normal.SetNormalize();
        return true;
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

    void ColliderPolyhedron::CalculateMinMaxBound()
    {
        m_max_bound = Math::REAL_NEGATIVE_MAX;
        m_min_bound = Math::REAL_POSITIVE_MAX;
        size_t size = m_vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                Real value = m_vertices->at(i)[j];
                if (m_max_bound[j] < value)
                {
                    m_max_bound[j] = value;
                }
                if (m_min_bound[j] > value)
                {
                    m_min_bound[j] = value;
                }
            }
        }
    }

    size_t ColliderPolyhedron::CreateSimplex(const std::vector<Vector3>& vertices)
    {
        //1. For each principle direction(X - axis, Y - axis, Z - axis)
        Vector3 min_vertex, max_vertex;
        bool    b_succeed = false;
        for (size_t i = 0; i < 3; ++i)
        {
            //  1. Find the minimum and maximum point in that dimension
            Real min = Math::REAL_POSITIVE_MAX;
            Real max = Math::REAL_NEGATIVE_MAX;
            for (auto& vertex : vertices)
            {
                if (vertex[i] < min)
                {
                    min        = vertex[i];
                    min_vertex = vertex;
                }
                if (vertex[i] > max)
                {
                    max        = vertex[i];
                    max_vertex = vertex;
                }
            }
            //  2. If min != max then break out of loop with success
            if (min != max)
            {
                b_succeed = true;
                break;
            }
            //  3. Continue the loop
        }
        //2. If the loop finished without success return and report that we have a degenerate point cloud, either 1D or 2D.
        if (b_succeed == false)
        {
            return 0;
        }

        //3. Find the point which is furthest distant from the line defined by the first two points.If this maximum distance is zero, then our point cloud is 1D, so report the degeneracy.
        Segment segment(min_vertex, max_vertex);
        Real    max_distance = 0.0f;
        Real    t            = 0.0f;
        Vector3 max_triangle;
        for (auto& vertex : vertices)
        {
            Real distance = segment.DistanceSquared(vertex, t);
            if (distance > max_distance)
            {
                max_distance = distance;
                max_triangle = vertex;
            }
        }
        if (Utility::IsZero(max_distance))
        {
            return 1;
        }
        
        //4. Find the point which has the largest absolute distance from the plane defined by the first three points.If this maximum distance is zero, then our point cloud is 2D, so report the the degeneracy.
        Triangle triangle;
        triangle.vertices[0] = min_vertex;
        triangle.vertices[1] = max_vertex;
        triangle.vertices[2] = max_triangle;
        //5. Create the tetrahedron polyhedron.Remember that if the distance from the plane of the fourth point was negative, then the order of the first three vertices must be reversed.
        //6. Return the fact that we created a non - degenerate tetrahedron of dimension 3.
        return 3;
    }

    void ColliderPolyhedron::AddToOutsideSet()
    {
    }

    void ColliderPolyhedron::CalculateHorizon()
    {
    }
}
