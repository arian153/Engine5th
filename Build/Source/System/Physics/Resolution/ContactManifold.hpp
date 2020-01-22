#pragma once
#include "../../Math/Math.hpp"
#include "ContactPoint.hpp"

namespace Engine5
{
    class ContactPoint;
    class RigidBody;

    class ContactManifold
    {
    public:
        ContactManifold();
        ~ContactManifold();
        void Set(const ContactManifold& manifold);
        void SetPersistentThreshold(Real threshold);
        void UpdateInvalidContact();
        void UpdateCurrentManifold(const ContactPoint& new_contact);
        void UpdateCollisionState();
        void CutDownManifold();
        size_t ContactsCount() const;

    private:
        Real DistanceFromPoint(const ContactPoint& contact, ContactPoint* p0);
        Real DistanceFromLineSegment(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1);
        Real DistanceFromTriangle(const ContactPoint& contact, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2);
        bool OnTriangle(ContactPoint* point, ContactPoint* p0, ContactPoint* p1, ContactPoint* p2);
        void CalculateNormal();

    private:
        friend class Resolution;
        friend class NarrowPhase;
        friend class ContactConstraints;

    private:
        Real    persistent_threshold_squared = 4.0f;
        bool    is_collide = false;
        Vector3 manifold_normal;

        //data
        RigidBody* m_body_a = nullptr;
        RigidBody* m_body_b = nullptr;

        std::vector<ContactPoint> contacts;
    };
}
