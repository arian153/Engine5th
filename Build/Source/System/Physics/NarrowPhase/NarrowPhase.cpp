#include "NarrowPhase.hpp"
#include "../Dynamics/RigidBody.hpp"
#include "Polytope.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "ManifoldTable.hpp"

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

    void NarrowPhase::GenerateContact(std::list<ColliderPair>& potential_list, ManifoldTable* data_table, const ColorFlag& gjk_flag, const ColorFlag& epa_flag)
    {
        data_table->FilteringManifolds();
        for (auto& pair : potential_list)
        {
            Simplex      simplex;
            ColliderSet* set_a = pair.first->m_collider_set;
            ColliderSet* set_b = pair.second->m_collider_set;
            if (GJKCollisionDetection(pair.first, pair.second, simplex) == true)
            {
                //collider pair have a collision do epa and create collision.
                if (simplex.count < 4)
                {
                    BlowUpSimplexToTetrahedron(pair.first, pair.second, simplex);
                }
                if (gjk_flag.b_flag)
                {
                    m_primitive_renderer->DrawTetrahedron(
                                                          simplex.simplex_vertex_a.global,
                                                          simplex.simplex_vertex_b.global,
                                                          simplex.simplex_vertex_c.global,
                                                          simplex.simplex_vertex_d.global,
                                                          eRenderingMode::Line, gjk_flag.color);
                }
                Polytope polytope = Polytope(simplex);
                //draw gjk result simplex
                ContactPoint new_contact_data;
                if (EPAContactGeneration(pair.first, pair.second, polytope, new_contact_data) == true)
                {
                    //draw EPA result
                    if (epa_flag.b_flag)
                    {
                        for (auto& face : polytope.faces)
                        {
                            m_primitive_renderer->DrawTriangle(
                                                               polytope.vertices[face.a].global,
                                                               polytope.vertices[face.b].global,
                                                               polytope.vertices[face.c].global,
                                                               eRenderingMode::Line, epa_flag.color);
                        }
                    }
                    //send a event about start and persist.
                    ContactManifold* found = data_table->FindManifold(set_a, set_b);
                    if (found == nullptr)
                    {
                        found = data_table->CreateManifold(set_a, set_b);
                    }
                    data_table->SendHasCollision(set_a, set_b, found->is_collide);
                    found->UpdateCurrentManifold(new_contact_data);
                    found->CutDownManifold();
                    found->is_collide = true;
                    ////draw contact result.
                    //if (contact_flag.b_flag)
                    //{
                    //    Vector3    pos_a = new_contact_data.global_position_a;
                    //    Vector3    pos_b = new_contact_data.global_position_b;
                    //    Quaternion no_rotation;
                    //    m_primitive_renderer->DrawPrimitive(Sphere(pos_a, no_rotation, 0.1f), eRenderingMode::Face, contact_flag.color);
                    //    m_primitive_renderer->DrawPrimitive(Sphere(pos_b, no_rotation, 0.1f), eRenderingMode::Face, contact_flag.color);
                    //    m_primitive_renderer->DrawSegment(pos_a, pos_a + new_contact_data.normal, contact_flag.color);
                    //    m_primitive_renderer->DrawSegment(pos_b, pos_b + new_contact_data.normal, contact_flag.color);
                    //}
                }
                else
                {
                    //generate invalid contact do not copy data.
                    //send a event invalid.
                    data_table->SendInvalidCollision(set_a, set_b);
                }
            }
            else
            {
                //if gjk result false, they are not colliding each other.
                //send a event about none and end.
                ContactManifold* found = data_table->FindManifold(set_a, set_b);
                if (found != nullptr)
                {
                    data_table->SendNotCollision(set_a, set_b, found->is_collide);
                    found->is_collide = false;
                }
            }
        }
    }

    SupportPoint NarrowPhase::GenerateCSOSupport(ColliderPrimitive* a, ColliderPrimitive* b, const Vector3& direction)
    {
        RigidBody* body_a     = a->m_rigid_body;
        RigidBody* body_b     = b->m_rigid_body;
        Vector3    body_dir_a = body_a != nullptr ? body_a->WorldToLocalVector(direction) : direction;
        Vector3    body_dir_b = body_b != nullptr ? body_b->WorldToLocalVector(-direction) : -direction;
        Vector3    local_a    = a->Support(a->WorldToLocalVector(body_dir_a).Unit());
        Vector3    local_b    = b->Support(b->WorldToLocalVector(body_dir_b).Unit());
        Vector3    support_a  = body_a != nullptr ? body_a->LocalToWorldPoint(a->LocalToWorldPoint(local_a)) : a->LocalToWorldPoint(local_a);
        Vector3    support_b  = body_b != nullptr ? body_b->LocalToWorldPoint(b->LocalToWorldPoint(local_b)) : b->LocalToWorldPoint(local_b);
        return SupportPoint(support_a - support_b, local_a, local_b);
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

    bool NarrowPhase::EPAContactGeneration(ColliderPrimitive* a, ColliderPrimitive* b, Polytope& polytope, ContactPoint& result)
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
                    result.b_valid = false;
                    return false;
                }
                if (fabsf(u) > 1.0f || fabsf(v) > 1.0f || fabsf(w) > 1.0f)
                {
                    //if any of the barycentric coefficients have a magnitude greater than 1, 
                    //then the origin is not within the triangular prism described by 'triangle'
                    //thus, there is no collision here, return invalid result.
                    result.b_valid = false;
                    return false;
                }
                result.collider_a = a;
                result.collider_b = b;
                result.local_position_a
                        = u * polytope.vertices[closest_face.a].local_a
                        + v * polytope.vertices[closest_face.b].local_a
                        + w * polytope.vertices[closest_face.c].local_a;
                result.local_position_b
                        = u * polytope.vertices[closest_face.a].local_b
                        + v * polytope.vertices[closest_face.b].local_b
                        + w * polytope.vertices[closest_face.c].local_b;
                result.global_position_a
                        = a->m_rigid_body != nullptr
                              ? a->m_rigid_body->LocalToWorldPoint(a->LocalToWorldPoint(result.local_position_a))
                              : a->LocalToWorldPoint(result.local_position_a);
                result.global_position_b
                        = b->m_rigid_body != nullptr
                              ? b->m_rigid_body->LocalToWorldPoint(b->LocalToWorldPoint(result.local_position_b))
                              : b->LocalToWorldPoint(result.local_position_b);
                result.normal = closest_face.normal.Normalize();
                result.depth  = closest_face.distance;
                ComputeBasisQuaternion(result.normal, result.tangent_a, result.tangent_b);
                return true;
            }
            polytope.Push(p);
            polytope.Expand(p);
        }
        result.b_valid = false;
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
        tangent_a = normal.CrossProduct(axis).Normalize();
        tangent_b = normal.CrossProduct(tangent_a).Normalize();
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
        tangent_b = normal.CrossProduct(tangent_a).Normalize();
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
        Vector3  simplex_global[ 4 ] = {simplex.simplex_vertex_a.global, simplex.simplex_vertex_b.global, simplex.simplex_vertex_c.global, simplex.simplex_vertex_d.global};
        Vector3  simplex_local1[ 4 ] = {simplex.simplex_vertex_a.local_a, simplex.simplex_vertex_b.local_a, simplex.simplex_vertex_c.local_a, simplex.simplex_vertex_d.local_a};
        Vector3  simplex_local2[ 4 ] = {simplex.simplex_vertex_a.local_b, simplex.simplex_vertex_b.local_b, simplex.simplex_vertex_c.local_b, simplex.simplex_vertex_d.local_b};
        unsigned num_vertices        = simplex.count;
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
                simplex_local1[1] = temp.local_a;
                simplex_local2[1] = temp.local_b;
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
            rot_case2.SetRotation(line_vec_case2, Math::PI_DIV_3);
            // find up to 6 directions perpendicular to the line vector
            // until a good search direction is used
            for (int i = 0; i < 6; ++i)
            {
                temp              = GenerateCSOSupport(collider_a, collider_b, search_dir_case2);
                simplex_global[2] = temp.global;
                simplex_local1[2] = temp.local_a;
                simplex_local2[2] = temp.local_b;
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
            simplex_local1[3] = temp.local_a;
            simplex_local2[3] = temp.local_b;
            // search direction not good, use its opposite direction
            if (simplex_global[3].LengthSquared() < Math::EPSILON_SQUARED)
            {
                search_dir_case3  = -search_dir_case3;
                temp              = GenerateCSOSupport(collider_a, collider_b, search_dir_case3);
                simplex_global[3] = temp.global;
                simplex_local1[3] = temp.local_a;
                simplex_local2[3] = temp.local_b;
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
