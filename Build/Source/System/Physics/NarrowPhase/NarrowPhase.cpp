#include "NarrowPhase.hpp"
#include "../Dynamics/RigidBody.hpp"
#include "Polytope.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "CollisionDataTable.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    NarrowPhase::NarrowPhase()
    {
    }

    NarrowPhase::~NarrowPhase()
    {
    }

    void NarrowPhase::Initialize()
    {
    }

    void NarrowPhase::Shutdown()
    {
    }

    void NarrowPhase::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        m_primitive_renderer = primitive_renderer;
    }

    void NarrowPhase::GenerateContact(std::unordered_map<size_t, Manifold>& manifold_table, CollisionDataTable* data_table, bool b_draw_gjk_flag, bool b_draw_epa_flag, bool b_draw_contact_flag)
    {
        for (auto& manifold : manifold_table)
        {
            Simplex simplex;
            data_table->ValidateKeyMap(manifold.second.collider_a, manifold.second.collider_b);
            if (GJKCollisionDetection(manifold.second.collider_a, manifold.second.collider_b, simplex) == true)
            {
                //collider pair have a collision do epa and create collision.
                if (simplex.count < 4)
                {
                    BlowUpSimplexToTetrahedron(manifold.second.collider_a, manifold.second.collider_b, simplex);
                }
                Polytope polytope = Polytope(simplex);
                //draw gjk result simplex
                if (b_draw_gjk_flag)
                {
                    //m_primitive_renderer->DrawTetrahedronWireFrame(simplex.simplex_vertex_a.global, simplex.simplex_vertex_b.global, simplex.simplex_vertex_c.global, simplex.simplex_vertex_d.global, Color(0.0f, 1.0f, 0.0f, 1.0f));
                }
                Contact new_contact_data;
                if (EPAContactGeneration(manifold.second.collider_a, manifold.second.collider_b, polytope, new_contact_data) == true)
                {
                    if (b_draw_epa_flag)
                    {
                        /*for (auto& face : polytope.faces)
                        {
                            m_primitive_renderer->DrawTriangleWireFrame(polytope.vertices[face.a].global, polytope.vertices[face.b].global, polytope.vertices[face.c].global, Color(1.0f, 1.0f, 0.0f, 1.0f));
                        }*/
                    }
                }
                if (new_contact_data.is_valid == false)
                {
                    //generate invalid contact do not copy data.
                    //send a event invalid.
                    //data_table->SendInvalidCollision(manifold.second.collider_a, manifold.second.collider_b);
                }
                else
                {
                    //send a event about start and persist.
                    //data_table->SendHasCollision(manifold.second.collider_a, manifold.second.collider_b, manifold.second.is_collide, new_contact_data.global_position_a, new_contact_data.global_position_b);
                    manifold.second.UpdateInvalidContact();
                    new_contact_data.is_collide = true;
                    manifold.second.UpdateCurrentManifold(new_contact_data);
                    manifold.second.CutDownManifold();
                    manifold.second.is_collide = true;
                    //draw contact result.
                    //E4_OUTPUT(new_contact_data.normal, "\n");
                    Vector3 pos_a = new_contact_data.global_position_a;
                    Vector3 pos_b = new_contact_data.global_position_b;
                    if (b_draw_contact_flag)
                    {
                        /*m_primitive_renderer->DrawSphereWireFrame(pos_a, 0.1f, Quaternion(), Color(1.0f, 0.5f, 0.0f, 1.0f));
                        m_primitive_renderer->DrawSphereWireFrame(pos_b, 0.1f, Quaternion(), Color(1.0f, 0.5f, 0.0f, 1.0f));
                        m_primitive_renderer->DrawLine(pos_a, pos_a + new_contact_data.normal, Color(1.0f, 0.5f, 0.0f, 1.0f));
                        m_primitive_renderer->DrawLine(pos_b, pos_b + new_contact_data.normal, Color(1.0f, 0.5f, 0.0f, 1.0f));*/
                    }
                }
            }
            else
            {
                //send a event about none and end.
                //data_table->SendNotCollision(manifold.second.collider_a, manifold.second.collider_b, manifold.second.is_collide);
                manifold.second.is_collide = false;
                manifold.second.UpdateCollisionState();
            }
        }
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

    bool NarrowPhase::EPAContactGeneration(ColliderPrimitive* a, ColliderPrimitive* b, Polytope& polytope, Contact& result)
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
        Quaternion rotation(normal, Math::Vector3::Y_AXIS);
        tangent_a = rotation.Rotate(Math::Vector3::X_AXIS);
        tangent_a.SetNormalize();
        tangent_b = rotation.Rotate(Math::Vector3::Z_AXIS);
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

    size_t NarrowPhase::FindLeastSignificantComponent(const Vector3& vector3)
    {
        Real    abs_x = fabsf(vector3.x);
        Real    abs_y = fabsf(vector3.y);
        Real    abs_z = fabsf(vector3.z);
        Vector3 axis(0.0f, 0.0f, 0.0f);
        if (abs_x > abs_y)
        {
            if (abs_x > abs_z)
                return 0;
            return 2;
        }
        if (abs_y > abs_z)
            return 1;
        return 2;
    }


    void NarrowPhase::BlowUpSimplexToTetrahedron(ColliderPrimitive* collider_a, ColliderPrimitive* collider_b, const Simplex& simplex)
    {
        Vector3  simplex_global[4] = {simplex.simplex_vertex_a.global, simplex.simplex_vertex_b.global, simplex.simplex_vertex_c.global, simplex.simplex_vertex_d.global};
        Vector3  simplex_local1[4] = {simplex.simplex_vertex_a.local1, simplex.simplex_vertex_b.local1, simplex.simplex_vertex_c.local1, simplex.simplex_vertex_d.local1};
        Vector3  simplex_local2[4] = {simplex.simplex_vertex_a.local2, simplex.simplex_vertex_b.local2, simplex.simplex_vertex_c.local2, simplex.simplex_vertex_d.local2};
        unsigned num_vertices      = simplex.count;
        // blow up simplex to tetrahedron
        Vector3      line_vec_case2;
        Vector3      search_dir_case2;
        Vector3      search_dir_case3;
        Matrix33     rot_case2;
        size_t       least_significant_axis;
        SupportPoint temp;

        // intentional omission of "break" statements for case fall-through
        // ReSharper disable CppDefaultCaseNotHandledInSwitchStatement
        switch (num_vertices)
            // ReSharper restore CppDefaultCaseNotHandledInSwitchStatement
        {
        case 1:
            // iterate until a good search direction is used
            for (auto& search_dir : m_search_dirs)
            {
                temp              = GenerateCSOSupport(collider_a, collider_b, search_dir);
                simplex_global[1] = temp.global;
                simplex_local1[1] = temp.local1;
                simplex_local2[1] = temp.local2;
                // good search direction used, break
                if ((simplex_global[1] - simplex_global[0]).LengthSquared() >= Math::EPSILON_SQUARED)
                {
                    break;
                }
            }
        case 2:
            // line direction vector
            line_vec_case2 = simplex_global[1] - simplex_global[0];

            // find least significant axis of line direction
            // 0 = x, 1 = y, 2 = z
            least_significant_axis = FindLeastSignificantComponent(line_vec_case2);

            // initial search direction
            search_dir_case2 = line_vec_case2.CrossProduct(m_axes[least_significant_axis]);

            // build a rotation matrix of 60 degrees about line vector
            rot_case2.SetTransformationRotation(line_vec_case2, Math::PI_DIV_3);

            // find up to 6 directions perpendicular to the line vector
            // until a good search direction is used
            for (int i = 0; i < 6; ++i)
            {
                temp              = GenerateCSOSupport(collider_a, collider_b, search_dir_case2);
                simplex_global[2] = temp.global;
                simplex_local1[2] = temp.local1;
                simplex_local2[2] = temp.local2;

                // good search direction used, break
                if (simplex_global[2].LengthSquared() > Math::EPSILON_SQUARED)
                    break;

                // rotate search direction by 60 degrees
                search_dir_case2 = rot_case2 * search_dir_case2;
            }
        case 3:
            // use triangle normal as search direction
            const Vector3 v01 = simplex_global[1] - simplex_global[0];
            const Vector3 v02 = simplex_global[2] - simplex_global[0];
            search_dir_case3  = v01.CrossProduct(v02);
            temp              = GenerateCSOSupport(collider_a, collider_b, search_dir_case3);
            simplex_global[3] = temp.global;
            simplex_local1[3] = temp.local1;
            simplex_local2[3] = temp.local2;

            // search direction not good, use its opposite direction
            if (simplex_global[3].LengthSquared() < Math::EPSILON_SQUARED)
            {
                search_dir_case3  = -search_dir_case3;
                temp              = GenerateCSOSupport(collider_a, collider_b, search_dir_case3);
                simplex_global[3] = temp.global;
                simplex_local1[3] = temp.local1;
                simplex_local2[3] = temp.local2;
            }
        }

        // fix tetrahedron winding
        // so that simplex[0]-simplex[1]-simplex[2] is CCW winding
        const Vector3 v30 = simplex_global[0] - simplex_global[3];
        const Vector3 v31 = simplex_global[1] - simplex_global[3];
        const Vector3 v32 = simplex_global[2] - simplex_global[3];
        const Real    det = v30.DotProduct(v31.CrossProduct(v32));
        if (det > 0.0f)
        {
            std::swap(simplex_global[0], simplex_global[1]);
            std::swap(simplex_local1[0], simplex_local1[1]);
            std::swap(simplex_local2[0], simplex_local2[1]);
        }
    }
}
