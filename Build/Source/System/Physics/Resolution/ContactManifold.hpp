#pragma once
#include "../../Math/Math.hpp"
#include "ContactPoint.hpp"
#include "../Utility/PhysicsDef.hpp"

namespace Engine5
{
    class ContactPoint;
    class RigidBody;
    class ColliderSet;

    class ContactManifold
    {
    public:
        ContactManifold();
        explicit ContactManifold(ColliderSet* a, ColliderSet* b);
        ~ContactManifold();
        ContactManifold(const ContactManifold& rhs);
        ContactManifold& operator=(const ContactManifold& rhs);

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
        friend class ManifoldTable;

    private:
        Real    persistent_threshold_squared = Collision::PERSISTENT_THRESHOLD_SQUARED;
        bool    is_collide = false;
        Vector3 manifold_normal;

        //data
        ColliderSet* m_set_a = nullptr;
        ColliderSet* m_set_b = nullptr;

        std::vector<ContactPoint> contacts;
    };
}
