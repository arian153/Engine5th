#include "Polyhedron.hpp"

#include <list>

#include "../../../Physics/ColliderPrimitive/OutsideSetFace.hpp"
#include "../../Utility/Utility.hpp"
#include "../ConvexHull2D/Triangle.hpp"
#include "../Others/Plane.hpp"
#include "../Others/Segment.hpp"

namespace Engine5
{
    Polyhedron::Polyhedron()
    {
        type = ePrimitiveType::Polyhedron;
    }

    Polyhedron::~Polyhedron()
    {
        if (m_vertices != nullptr)
        {
            Shutdown();
        }
    }

    void Polyhedron::Initialize()
    {
        m_vertices = new std::vector<Vector3>();
    }

    void Polyhedron::Shutdown()
    {
        if (m_vertices != nullptr)
        {
            m_vertices->clear();
            delete m_vertices;
            m_vertices = nullptr;
        }
    }

    void Polyhedron::SetUnit()
    {
        Vector3 scale = max_point - min_point;
        scale.SetInverse();

        if (m_vertices != nullptr)
        {
            for (auto& vertex : *m_vertices)
            {
                vertex = vertex.HadamardProduct(scale);
            }
        }
    }

    Vector3 Polyhedron::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (auto& vertex : *m_vertices)
        {
            Real projection = vertex.DotProduct(direction);
            if (projection > p)
            {
                result = vertex;
                p      = projection;
            }
        }
        return result;
    }

    bool Polyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 Polyhedron::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    void Polyhedron::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
    }

    void Polyhedron::UpdateMinMaxPoint()
    {
        min_point = Vector3(Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX);
        max_point = Vector3(Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX);

        for (auto& vertex : *m_vertices)
        {
            if (vertex.x < min_point.x)
            {
                min_point.x = vertex.x;
            }

            if (vertex.y < min_point.y)
            {
                min_point.y = vertex.y;
            }

            if (vertex.z < min_point.z)
            {
                min_point.z = vertex.z;
            }

            if (vertex.x > max_point.x)
            {
                max_point.x = vertex.x;
            }

            if (vertex.y > max_point.y)
            {
                max_point.y = vertex.y;
            }

            if (vertex.z > max_point.z)
            {
                max_point.z = vertex.z;
            }
        }
    }

    size_t Polyhedron::CreateSimplex(const std::vector<Vector3>& vertices) const
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
        if (Math::IsZero(max_distance))
        {
            return 1;
        }
        //4. Find the point which has the largest absolute distance from the plane defined by the first three points.If this maximum distance is zero, then our point cloud is 2D, so report the the degeneracy.
        max_distance = 0.0f;
        Vector3 max_tetrahedron;
        for (auto& vertex : vertices)
        {
            Real distance = Triangle::DistanceSquared(vertex, min_vertex, max_vertex, max_triangle);
            if (distance > max_distance)
            {
                max_distance    = distance;
                max_tetrahedron = vertex;
            }
        }
        if (Math::IsZero(max_distance))
        {
            return 2;
        }
        //5. Create the tetrahedron polyhedron.Remember that if the distance from the plane of the fourth point was negative, then the order of the first three vertices must be reversed.
        Plane plane;
        plane.Set(min_vertex, max_vertex, max_triangle);
        Real order = plane.PlaneTest(max_tetrahedron);
        if (order > 0.0f)
        {
            m_vertices->push_back(min_vertex);
            m_vertices->push_back(max_vertex);
            m_vertices->push_back(max_triangle);
            m_vertices->push_back(max_tetrahedron);
        }
        else
        {
            m_vertices->push_back(max_triangle);
            m_vertices->push_back(max_vertex);
            m_vertices->push_back(min_vertex);
            m_vertices->push_back(max_tetrahedron);
        }
        //6. Return the fact that we created a non - degenerate tetrahedron of dimension 3.
        m_faces->emplace_back(0, 1, 2);
        m_faces->emplace_back(1, 2, 3);
        m_faces->emplace_back(2, 3, 0);
        m_faces->emplace_back(3, 0, 1);
        return 3;
    }

    void Polyhedron::AddToOutsideSet(std::vector<Vector3>& vertices, OutsideSetFace& result) const
    {
        Vector3 face_a(m_vertices->at(result.a));
        Vector3 face_b(m_vertices->at(result.b));
        Vector3 face_c(m_vertices->at(result.c));

        Plane plane(face_a, face_b, face_c);

        auto begin = vertices.begin();
        auto end   = vertices.end();

        std::list<std::vector<Vector3>::iterator> erase_list;
        //1. For each vertex in the listVertices
        for (auto it = begin; it != end; ++it)
        {
            //1. distance = Distance from the plane of the face to the vertex
            Real distance = plane.Distance(*it);
            //2. If the vertex is in the plane of the face(i.e.distance == 0)
            if (Math::IsZero(distance))
            {
                //3. Then check to see if the vertex is coincident with any vertices of the face, and if so merge it into that face vertex.
                if (Triangle::IsContainPoint(*it, face_a, face_b, face_c))
                {
                    //merge
                    erase_list.push_back(it);
                }
            }
                //4. Else If distance > 0
            else if (distance > 0.0f)
            {
                //5. Then claim the vertex by removing it from this list and adding it the face's set of outside vertices
                result.outside_vertices.push_back(*it);
                erase_list.push_back(it);
            }
            //6. Continue the loop
        }
        for (auto& it : erase_list)
        {
            vertices.erase(it);
        }
    }

    void Polyhedron::CalculateHorizon()
    {
    }
}
