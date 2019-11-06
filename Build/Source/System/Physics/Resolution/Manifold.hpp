#pragma once
#include "../../Math/Math.hpp"
#include "Contact.hpp"

namespace Engine5
{
    class Contact;

    class Manifold
    {
    public:
        Manifold();
        ~Manifold();
        void Set(const Manifold& manifold);
        void SetPersistentThreshold(Real threshold);
        void UpdateInvalidContact();
        void UpdateCurrentManifold(const Contact& new_contact);
        void UpdateCollisionState();
        void CutDownManifold();

        size_t ContactsCount() const;

    private:
        Real DistanceFromPoint(const Contact& contact, Contact* p0);
        Real DistanceFromLineSegment(const Contact& contact, Contact* p0, Contact* p1);
        Real DistanceFromTriangle(const Contact& contact, Contact* p0, Contact* p1, Contact* p2);
        bool OnTriangle(Contact* point, Contact* p0, Contact* p1, Contact* p2);

    private:
        friend class Resolution;

    private:
        Real    normal_impulse_sum           = 0.0f;
        Real    tangent_a_impulse_sum        = 0.0f;
        Real    tangent_b_impulse_sum        = 0.0f;
        Real    persistent_threshold_squared = 4.0f;
        Vector3 cache_a_positional;
        Vector3 cache_a_rotational;
        Vector3 cache_b_positional;
        Vector3 cache_b_rotational;
        bool    is_collide = false;

        //data
        ColliderPrimitive*   collider_a = nullptr;
        ColliderPrimitive*   collider_b = nullptr;
        std::vector<Contact> contacts;
    };
}
