#include "Manifold.hpp"
#include <list>
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    Manifold::Manifold()
    {
    }

    Manifold::~Manifold()
    {
    }

    void Manifold::Set(const Manifold& manifold)
    {
        this->collider_a                   = manifold.collider_a;
        this->collider_b                   = manifold.collider_b;
        this->normal_impulse_sum           = manifold.normal_impulse_sum;
        this->tangent_a_impulse_sum        = manifold.tangent_a_impulse_sum;
        this->tangent_b_impulse_sum        = manifold.tangent_b_impulse_sum;
        this->persistent_threshold_squared = manifold.persistent_threshold_squared;
        this->is_collide                   = manifold.is_collide;
        for (auto& contact : manifold.contacts)
        {
            this->contacts.push_back(contact);
        }
    }

    void Manifold::SetPersistentThreshold(Real threshold)
    {
        persistent_threshold_squared = threshold * threshold;
    }

    void Manifold::UpdateInvalidContact()
    {
        //erase contact list.
        std::list<Contact*> remove_list;
        for (auto& contact : contacts)
        {
            auto    body_a            = collider_a->GetRigidBody();
            auto    body_b            = collider_b->GetRigidBody();
            Vector3 local_to_global_a = body_a->LocalToWorldPoint(contact.local_position_a);
            Vector3 local_to_global_b = body_b->LocalToWorldPoint(contact.local_position_b);
            Vector3 r_ab              = local_to_global_b - local_to_global_a;
            Vector3 r_a               = contact.global_position_a - local_to_global_a;
            Vector3 r_b               = contact.global_position_b - local_to_global_b;
            bool    still_penetrating = contact.normal.DotProduct(r_ab) <= 0.0f;
            bool    r_a_close_enough  = r_a.LengthSquared() < persistent_threshold_squared;
            bool    r_b_close_enough  = r_b.LengthSquared() < persistent_threshold_squared;

            // keep contact point if the collision pair is still colliding at this point, 
            // and the local positions are not too far from the global positions original acquired from collision detection
            if (r_a_close_enough && r_b_close_enough && still_penetrating)
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

    void Manifold::UpdateCurrentManifold(const Contact& new_contact)
    {
        bool add_contact = false;
        for (auto& contact : contacts)
        {
            Vector3 r_a            = new_contact.global_position_a - contact.global_position_a;
            Vector3 r_b            = new_contact.global_position_b - contact.global_position_b;
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

    void Manifold::UpdateCollisionState()
    {
        for (auto& contact : contacts)
        {
            contact.is_collide = is_collide;
        }
    }

    void Manifold::CutDownManifold()
    {
        if (contacts.size() < 4)
        {
            return;
        }

        // find the deepest penetrating one
        Contact* deepest     = nullptr;
        Real     penetration = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            if (contact.depth > penetration)
            {
                penetration = contact.depth;
                deepest     = &contact;
            }
        }

        // find second contact
        Contact* furthest1         = nullptr;
        Real     distance_squared1 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromPoint(contact, deepest);
            if (dist > distance_squared1)
            {
                distance_squared1 = dist;
                furthest1         = &contact;
            }
        }

        // find third contact
        Contact* furthest2         = nullptr;
        float    distance_squared2 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromLineSegment(contact, deepest, furthest1);
            if (dist > distance_squared2)
            {
                distance_squared2 = dist;
                furthest2         = &contact;
            }
        }

        // find fourth contact
        Contact* furthest3         = nullptr;
        float    distance_squared3 = Math::REAL_NEGATIVE_MAX;
        for (auto& contact : contacts)
        {
            Real dist = DistanceFromTriangle(contact, deepest, furthest1, furthest2);
            if (dist > distance_squared3)
            {
                distance_squared3 = dist;
                furthest3         = &contact;
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

    size_t Manifold::ContactsCount() const
    {
        return contacts.size();
    }

    Real Manifold::DistanceFromPoint(const Contact& contact, Contact* p0)
    {
        if (p0 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        return (contact.global_position_a - p0->global_position_a).LengthSquared();
    }

    Real Manifold::DistanceFromLineSegment(const Contact& contact, Contact* p0, Contact* p1)
    {
        if (p0 == nullptr || p1 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        Vector3 n  = p1->global_position_a - p0->global_position_a;
        Vector3 pa = p0->global_position_a - contact.global_position_a;
        Vector3 c  = n * (pa.DotProduct(n) / n.DotProduct(n));
        Vector3 d  = pa - c;
        return d.DotProduct(d);
    }

    Real Manifold::DistanceFromTriangle(const Contact& contact, Contact* p0, Contact* p1, Contact* p2)
    {
        if (p0 == nullptr || p1 == nullptr || p2 == nullptr)
        {
            return Math::REAL_NEGATIVE_MAX;
        }
        Vector3 v0      = p1->global_position_a - p0->global_position_a;
        Vector3 v1      = p2->global_position_a - p0->global_position_a;
        Vector3 v2      = contact.global_position_a - p0->global_position_a;
        Real    d00     = v0.DotProduct(v0);
        Real    d01     = v0.DotProduct(v1);
        Real    d11     = v1.DotProduct(v1);
        Real    d20     = v2.DotProduct(v0);
        Real    d21     = v2.DotProduct(v1);
        Real    denom   = d00 * d11 - d01 * d01;
        Real    v       = (d11 * d20 - d01 * d21) / denom;
        Real    w       = (d00 * d21 - d01 * d20) / denom;
        Real    u       = 1.0f - v - w;
        Vector3 closest = p0->global_position_a * u + p1->global_position_a * v + p2->global_position_a * w;
        return (closest - contact.global_position_a).LengthSquared();
    }

    bool Manifold::OnTriangle(Contact* point, Contact* p0, Contact* p1, Contact* p2)
    {
        if (point == nullptr || p0 == nullptr || p1 == nullptr || p2 == nullptr)
        {
            return false;
        }
        Vector3 edge1  = p1->global_position_a - p0->global_position_a;
        Vector3 edge2  = p2->global_position_a - p1->global_position_a;
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
        w_test        = edge3.CrossProduct(point->global_position_a - p2->global_position_a);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        return true;
    }
}
