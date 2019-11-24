#include "NarrowPhase.hpp"
#include "../Dynamics/RigidBody.hpp"
#include "Polytope.hpp"

namespace Engine5
{
    NarrowPhase::NarrowPhase()
    {
    }

    NarrowPhase::~NarrowPhase()
    {
    }

    void NarrowPhase::GenerateContact(bool b_draw)
    {

    }

    SupportPoint NarrowPhase::GenerateCSOSupport(ColliderPrimitive* a, ColliderPrimitive* b, const Vector3& direction)
    {
        RigidBody* body_a      = a->m_rigid_body;
        RigidBody* body_b      = b->m_rigid_body;
        Vector3    local_dir_a = body_a->WorldToLocalVector(direction);
        Vector3    local_dir_b = body_b->WorldToLocalVector(-direction);
        Vector3    support_a;
        Vector3    support_b;
        return SupportPoint(body_a->LocalToWorldPoint(support_a) - body_a->LocalToWorldPoint(support_b), support_a, support_b);
    }

    bool NarrowPhase::GJKCollisionDetection(ColliderPrimitive* a, ColliderPrimitive* b, Simplex& simplex)
    {
        Vector3 direction        = Vector3(0.0f, 1.0f, 0.0f);
        int     exit_iteration   = 0;
        simplex.simplex_vertex_a = GenerateCSOSupport(a, b, direction);
        simplex.DoSimplexPoint(direction);

        //avoid infinite loop
        while (exit_iteration < 50)
        {
            simplex.simplex_vertex_a = GenerateCSOSupport(a, b, direction);
            if (simplex.simplex_vertex_a.global.DotProduct(direction) < 0.0f)
            {
                return false;
            }
            else
            {
                if (simplex.IsContainOrigin(direction) == true)
                {
                    return true;
                }
            }
            exit_iteration++;
        }
        return false;
    }

    bool NarrowPhase::EPAContactGeneration(ColliderPrimitive* a, ColliderPrimitive* b, Polytope& polytope, Contact result)
    {
        int exit_iteration = 0;
        while (exit_iteration < 100)
        {
            PolytopeFace closest_face = polytope.PickClosestFaceOfPolytopeToOrigin();
            SupportPoint p            = GenerateCSOSupport(a, b, closest_face.normal);
            Real         distance     = p.global.DotProduct(closest_face.normal);
            if (distance - closest_face.distance < Math::EPSILON)
            {
                Real u, v, w;
                closest_face.BarycentricCoordinates(closest_face.normal * closest_face.distance, u, v, w, &polytope);
                if (Utility::IsValid(u) == false || Utility::IsValid(v) == false || Utility::IsValid(w) == false)
                {
                    //barycentric can fail and generate invalid coordinates, if this happens return invalid result.
                    result.is_valid = false;
                    return false;
                }
                if (fabsf(u) > 1.0f || fabsf(v) > 1.0f || fabsf(w) > 1.0f)
                {
                    //if any of the barycentric coefficients have a magnitude greater than 1, 
                    //then the origin is not within the triangular prism described by 'triangle'
                    //thus, there is no collision here, return invalid result.
                    result.is_valid = false;
                    return false;
                }
                result.collider_a       = a;
                result.collider_b       = b;
                result.local_position_a = (u * polytope.vertices[closest_face.a].local1)
                        + (v * polytope.vertices[closest_face.b].local1) + (w * polytope.vertices[closest_face.c].local1);
                result.local_position_b = (u * polytope.vertices[closest_face.a].local2)
                        + (v * polytope.vertices[closest_face.b].local2) + (w * polytope.vertices[closest_face.c].local2);
                result.global_position_a = a->m_rigid_body->LocalToWorldPoint(result.local_position_a);
                result.global_position_b = b->m_rigid_body->LocalToWorldPoint(result.local_position_b);
                result.normal            = closest_face.normal;
                result.depth             = closest_face.distance;
                result.normal.SetNormalize();
                ComputeBasisQuaternion(result.normal, result.tangent_a, result.tangent_b);
                return true;
            }
            polytope.Push(p);
            polytope.Expand(p);
        }
        result.is_valid   = false;
        result.is_collide = false;
        return false;
    }

    void NarrowPhase::ComputeBasisQuaternion(const Vector3& normal, Vector3& tangent_a, Vector3& tangent_b)
    {
        Quaternion rotation(normal, Vector3::AxisY());
        tangent_a = rotation.Rotate(Vector3::AxisX());
        tangent_a.SetNormalize();
        tangent_b = rotation.Rotate(Vector3::AxisZ());
        tangent_b.SetNormalize();
    }

    void NarrowPhase::ComputeBasisCrossProduct(const Vector3& normal, Vector3& tangent_a, Vector3& tangent_b)
    {
        // find least axis of least significant component
        Real    abs_x = fabsf(normal.x);
        Real    abs_y = fabsf(normal.y);
        Real    abs_z = fabsf(normal.z);
        Vector3 axis(0.0f, 0.0f, 0.0f);
        if (abs_x > abs_y)
        {
            if (abs_x > abs_z)
                axis.x = 1.0f; // X > Y > Z, X > Z > Y
            else
                axis.z = 1.0f; // Z > X > Y
        }
        else
        {
            if (abs_y > abs_z)
                axis.y = 1.0f; // Y > X > Z, Y > Z > X
            else
                axis.z = 1.0f; // Z > Y > X
        }

        // compute tangents
        tangent_a = normal.CrossProduct(axis).Unit();
        tangent_b = normal.CrossProduct(tangent_a).Unit();
    }

    void NarrowPhase::ComputeBasisFast(const Vector3& normal, Vector3& tangent_a, Vector3& tangent_b)
    {
        //sqrt(1/3) = 0.57735
        if (fabsf(normal.x) >= 0.57735f)
        {
            tangent_a.Set(normal.y, -normal.x, 0.0f);
        }
        else
        {
            tangent_a.Set(0.0f, normal.z, -normal.y);
        }
        tangent_a.SetNormalize();
        tangent_b = normal.CrossProduct(tangent_a).Unit();
    }
}
