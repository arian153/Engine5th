#include "ContactManifold.hpp"
#include <list>
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    ContactManifold::ContactManifold()
    {
    }

    ContactManifold::~ContactManifold()
    {
    }

    void ContactManifold::Set(const ContactManifold& manifold)
    {
        this->collider_a = manifold.collider_a;
        this->collider_b = manifold.collider_b;
        this->persistent_threshold_squared = manifold.persistent_threshold_squared;
        this->is_collide = manifold.is_collide;
        for (auto& contact : manifold.contacts)
        {
            this->contacts.push_back(contact);
        }
    }

    void ContactManifold::SetPersistentThreshold(Real threshold)
    {
        persistent_threshold_squared = threshold * threshold;
    }

    void ContactManifold::UpdateInvalidContact()
    {
        //erase contact list.
        std::list<ContactPoint*> remove_list;
        for (auto& contact : contacts)
        {
            auto    body_a = collider_a->GetRigidBody();
            auto    body_b = collider_b->GetRigidBody();

            //convert existing contact point from local space to world space.
            //if both bodies are far enough away, remove contact from manifold data.

            Vector3 local_to_global_a = body_a->LocalToWorldPoint(contact.local_position_a);
            Vector3 local_to_global_b = body_b->LocalToWorldPoint(contact.local_position_b);

            //current frame's distance between a to b.
            Vector3 r_ab = local_to_global_b - local_to_global_a;

            //how much distance changed between prev to current
            Vector3 r_a = contact.global_position_a - local_to_global_a;
            Vector3 r_b = contact.global_position_b - local_to_global_b;

            //check still penetrate between both bodies.
            bool    b_still_penetrating = contact.normal.DotProduct(r_ab) <= 0.0f;
            bool    b_r_a_close_enough = r_a.LengthSquared() < persistent_threshold_squared;
            bool    b_r_b_close_enough = r_b.LengthSquared() < persistent_threshold_squared;

            // keep contact point if the collision pair is still colliding at this point, 
            // and the local positions are not too far from the global positions original acquired from collision detection
            if (b_r_a_close_enough && b_r_b_close_enough && b_still_penetrating)
            {
                // contact persistent, keep
                contact.is_persistent = true;
            }
            else
            {
                // contact invalid, remove
                remove_list.push_back(&contact);
            }
        }
        for (auto it = remove_list.begin(); it != remove_list.end(); ++it)
        {
            for (auto itc = contacts.begin(); itc != contacts.end();)
            {
                if (*itc == **it)
                {
                    contacts.erase(itc++);
                    break;
                }
                ++itc;
            }
        }
        remove_list.clear();
    }

    void ContactManifold::UpdateCurrentManifold(const ContactPoint& new_contact)
    {
        bool add_contact = false;
        for (auto& contact : contacts)
        {
            Vector3 r_a = new_contact.global_position_a - contact.global_position_a;
            Vector3 r_b = new_contact.global_position_b - contact.global_position_b;
            bool    r_a_far_enough = r_a.LengthSquared() > persistent_threshold_squared;
            bool    r_b_far_enough = r_b.LengthSquared() > persistent_threshold_squared;

            // proximity check  
            if (r_a_far_enough && r_b_far_enough)
            {
                //add new_contact
                add_contact = true;
            }
        }
        if (add_contact == true || contacts.size() == 0)
        {
            contacts.push_back(new_contact);
        }
    }

    void ContactManifold::UpdateCollisionState()
    {
        for (auto& contact : contacts)
        {
            contact.is_collide = is_collide;
        }
    }

    void ContactManifold::CutDownManifold()
    {
        if (contacts.size() < 4)
        {
            return;
        }

        // find the deepest penetrating one
        ContactPoint* deepest = nullptr;
        Real     penetration = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            if (contact.depth > penetration)
            {
                penetration = contact.depth;
                deepest = &contact;
            }
        }

        // find second contact
        ContactPoint* furthest1 = nullptr;
        Real     distance_squared1 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromPoint(contact, deepest);
            if (dist > distance_squared1)
            {
                distance_squared1 = dist;
                furthest1 = &contact;
            }
        }

        // find third contact
        ContactPoint* furthest2 = nullptr;
        float    distance_squared2 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromLineSegment(contact, deepest, furthest1);
            if (dist > distance_squared2)
            {
                distance_squared2 = dist;
                furthest2 = &contact;
            }
        }

        // find fourth contact
        ContactPoint* furthest3 = nullptr;
        float    distance_squared3 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromTriangle(contact, deepest, furthest1, furthest2);
            if (dist > distance_squared3)
            {
                distance_squared3 = dist;
                furthest3 = &contact;
            }
        }
        contacts.clear();
        if (deepest != nullptr)
        {
            contacts.push_back(*deepest);
        }
        if (furthest1 != nullptr)
        {
            contacts.push_back(*furthest1);
        }
        if (furthest2 != nullptr)
        {
            contacts.push_back(*furthest2);
        }
        bool on_triangle = OnTriangle(furthest3, deepest, furthest1, furthest2);
        if (on_triangle == false && furthest3 != nullptr)
        {
            contacts.push_back(*furthest3);
        }
    }

    size_t ContactManifold::ContactsCount() const
    {
        return contacts.size();
    }

    Real ContactManifold::DistanceFromPoint(const ContactPoint& contact, ContactPoint* p0)
    {
        if (p0 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        return (contact.global_position_a - p0->global_position_a).LengthSquared();
    }

    Real ContactManifold::DistanceFromLineSegment(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1)
    {
        if (p0 == nullptr || p1 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        Vector3 n = p1->global_position_a - p0->global_position_a;
        Vector3 pa = p0->global_position_a - contact.global_position_a;
        Vector3 c = n * (pa.DotProduct(n) / n.DotProduct(n));
        Vector3 d = pa - c;
        return d.DotProduct(d);
    }

    Real ContactManifold::DistanceFromTriangle(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2)
    {
        if (p0 == nullptr || p1 == nullptr || p2 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        Vector3 v0 = p1->global_position_a - p0->global_position_a;
        Vector3 v1 = p2->global_position_a - p0->global_position_a;
        Vector3 v2 = contact.global_position_a - p0->global_position_a;
        Real    d00 = v0.DotProduct(v0);
        Real    d01 = v0.DotProduct(v1);
        Real    d11 = v1.DotProduct(v1);
        Real    d20 = v2.DotProduct(v0);
        Real    d21 = v2.DotProduct(v1);
        Real    denom = d00 * d11 - d01 * d01;
        Real    v = (d11 * d20 - d01 * d21) / denom;
        Real    w = (d00 * d21 - d01 * d20) / denom;
        Real    u = 1.0f - v - w;
        Vector3 closest = p0->global_position_a * u + p1->global_position_a * v + p2->global_position_a * w;
        return (closest - contact.global_position_a).LengthSquared();
    }

    bool ContactManifold::OnTriangle(ContactPoint* point, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2)
    {
        if (point == nullptr || p0 == nullptr || p1 == nullptr || p2 == nullptr)
        {
            return false;
        }
        Vector3 edge1 = p1->global_position_a - p0->global_position_a;
        Vector3 edge2 = p2->global_position_a - p1->global_position_a;
        Vector3 normal = edge1.CrossProduct(edge2);
        Vector3 w_test = edge1.CrossProduct(point->global_position_a - p0->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        w_test = edge2.CrossProduct(point->global_position_a - p1->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        Vector3 edge3 = p0->global_position_a - p2->global_position_a;
        w_test = edge3.CrossProduct(point->global_position_a - p2->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        return true;
    }

    void ContactManifold::CalculateNormal()
    {
        size_t contact_size = contacts.size();

        if (contact_size == 1)
        {
            manifold_normal = contacts.at(0).normal;
        }
        else if (contact_size == 2)
        {
            Vector3 line_dir = (contacts.at(1).global_position_a - contacts.at(0).global_position_a).Unit();
            Vector3 pos_t = line_dir.CrossProduct((collider_b->GetBodyPosition() - collider_a->GetBodyPosition()).Unit()).Unit();
            Vector3 c0n_t = line_dir.CrossProduct(contacts.at(0).normal).Unit();
            Vector3 c1n_t = line_dir.CrossProduct(contacts.at(1).normal).Unit();
            Vector3 tangent = (pos_t + c0n_t + c1n_t).Unit();
            manifold_normal = line_dir.CrossProduct(tangent).Unit();
        }
        else if (contact_size == 3 || contact_size == 4)
        {
            manifold_normal = Triangle::Normal(contacts.at(0).global_position_a, contacts.at(1).global_position_a, contacts.at(2).global_position_a);
        }
    }

}
